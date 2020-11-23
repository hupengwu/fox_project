#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <arpa/inet.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <pthread.h>
#include <sys/errno.h>    // for errno
#include <unistd.h>

#include <STLStringUtils.h>
#include <FoxLoggerFactory.h>

#include "FoxTcpServerSocket.h"
#include "FoxTcpServerRecver.h"
#include "FoxTcpSocketKey.h"

ILogger* FoxTcpServerSocket::logger =  FoxLoggerFactory::getLogger();

FoxTcpServerSocket::FoxTcpServerSocket()
{
    this->isExit = false;
    this->bFinished = true;
    this->socketHandler = nullptr;
    this->listenThread = nullptr;
    this->serverSocket = -1;

    this->socketHandler = new FoxTcpServerHandler();
}

FoxTcpServerSocket::~FoxTcpServerSocket()
{
    delete this->socketHandler;
}

bool FoxTcpServerSocket::start(int nSocketPort)
{
    // <1> 创建一个socket句柄
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket == -1)
    {
        logger->info("Create Socket Failed!");
        return false;
    }
    logger->info("socket create successfully!");

    // <2> 设置地址重用：避免刚刚已经调用close的端口无法被绑定（之前被使用过的socket，操作系统会保持一定的TIME_WAIT时间内禁止再次绑定 ）
    int on = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
    {
        logger->info("set SO_REUSEADDR failed");
        return false;
    }

    // <3> 绑定socket句柄和地址+端口
    bzero(&this->serverAddr, sizeof(struct sockaddr_in));
    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->serverAddr.sin_port = htons((u_short)nSocketPort);
    if (bind(serverSocket, (struct sockaddr*)&this->serverAddr, sizeof(struct sockaddr)) < 0)
    {
        std::string message = STLStringUtils::snprintf("Bind error.Port[%d]", this->serverAddr.sin_port);
        logger->info(message);
        return false;
    }

    // <4> 设置accept超时:1秒
    struct timeval timeout = { 1,0 };
    if (setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set accept timeout failed");
        return false;
    }

    // <5> serverSocket用于监听
    if (listen(serverSocket, 20))
    {
        logger->info("Listen error!");
        return false;
    }
    logger->info(STLStringUtils::snprintf("Listening on port[% d]", this->serverAddr.sin_port));

    // <6> 启动客户端数据处理的异步任务线程池
    this->clientThread.start(1);

    // <7> 启动一个专门监听接入的listener线程
    this->setFinished(false);
    this->listenThread = new thread(listenThreadFunc, ref(*this));

    return true;
}

void FoxTcpServerSocket::close()
{
    // 通知handler退出：handler处理完毕后，关闭客户端的socket
    this->socketHandler->setExit(true);
    
    // 关闭处理客户端的线程池
	this->clientThread.close();

    // 通知监听线程退出
    this->setExit(true);

    // 检查：全体线程是否运行结束
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    // 回收线程
    thread* thread = this->listenThread;    
    if (thread != nullptr)
    {
        if (thread->joinable())
        {
            thread->join();
        }
        delete this->listenThread;
        this->listenThread = nullptr;
    }

    // 重置标识
    this->setFinished(true);
    this->setExit(false);
    this->socketHandler->setExit(false);

    // 关闭服务端socket
    if (this->serverSocket != -1)
    {
        int result = ::shutdown(this->serverSocket, 0x02);
        result = ::close(this->serverSocket);
        this->serverSocket = -1;
    }
    
}

int FoxTcpServerSocket::getServerSocket()
{
	return this->serverSocket;
}

sockaddr_in FoxTcpServerSocket::getServerAddr()
{
    return this->serverAddr;
}

bool FoxTcpServerSocket::bindSocketHandler(FoxTcpServerHandler* socketHandler)
{
    lock_guard<mutex> guard(this->lock);

    if (socketHandler == nullptr)
    {
        return false;
    }

    delete this->socketHandler;
    this->socketHandler = socketHandler;

    return true;
}

void FoxTcpServerSocket::setFinished(bool finished)
{
    lock_guard<mutex> guard(this->lock);
    this->bFinished = finished;
}

bool FoxTcpServerSocket::getFinished()
{
    lock_guard<mutex> guard(this->lock);
    return this->bFinished;
}

void FoxTcpServerSocket::setExit(bool isExit)
{
    lock_guard<mutex> guard(this->lock);
    this->isExit = isExit;
}

bool FoxTcpServerSocket::getExit()
{
    lock_guard<mutex> guard(this->lock);
    return this->isExit;
}

void FoxTcpServerSocket::listenThreadFunc(FoxTcpServerSocket& socket)
{
    while (true)
    {
        // 检查：退出线程标记
        if (socket.getExit())
        {
            break;
        }

        //定义客户端的socket地址结构clientAddr
        sockaddr_in clientAddr;
        int ilength = sizeof(clientAddr);

        // 等待客户端socket的接入
        int hClientSocket = accept(socket.serverSocket, (sockaddr*)&clientAddr, (socklen_t*)(&ilength));
        if (-1 == hClientSocket)
        {            
            continue;
        }

        // 接入了一个客户socket
        string message = STLStringUtils::snprintf("try cconnect from client, address : %s, port : %d ,Socket Num : % d",
            inet_ntoa(clientAddr.sin_addr),
            clientAddr.sin_port,
            hClientSocket
        );
        logger->info(message);

        // 线程池是否繁忙状态
        if (socket.clientThread.isBusy())
        {
            // 关闭这个无法继续接入的socket
            ::shutdown(hClientSocket, 0x02);
            ::close(hClientSocket);

            string message = STLStringUtils::snprintf("disconnect from client, address : %s, port : %d ,Socket Num : % d",
                inet_ntoa(clientAddr.sin_addr),
                clientAddr.sin_port,
                hClientSocket
            );
            logger->info(message);

            continue;
        }
        
        // 发出一个任务去处理这个客户端接入：new出来的socketKey和clientAddr，在使用完后会被FoxTcpServerRecver自动回收，所以不需要主动释放
        FoxTcpSocketKey socketKey;
        socketKey.setSocket(hClientSocket);
        socketKey.setSocketAddr(clientAddr);        
        FoxTcpServerRecver* serverRecver = new FoxTcpServerRecver(socketKey, socket.socketHandler);
        socket.clientThread.execute(serverRecver);
    }

    // 退出线程
    string message = STLStringUtils::snprintf("finish listenThreadFunc from server, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(socket.getServerAddr().sin_addr),
        socket.getServerAddr().sin_port,
        socket.getServerSocket()
    );
    logger->info(message);

    socket.setFinished(true);
}