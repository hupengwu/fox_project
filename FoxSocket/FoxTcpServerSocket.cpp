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
#include "FoxSocketKey.h"

ILogger* FoxTcpServerSocket::logger =  FoxLoggerFactory::getLogger();

FoxTcpServerSocket::FoxTcpServerSocket()
{
    this->isExit = false;
    this->bFinished = true;
    this->socketHandler = nullptr;
    this->recvThread = nullptr;
    this->socketKey.setSocket(-1);
    this->nThreads = 5;

    this->socketHandler = new FoxSocketHandler();
}

FoxTcpServerSocket::~FoxTcpServerSocket()
{
    delete this->socketHandler;
}

bool FoxTcpServerSocket::start(int nSocketPort)
{
    // <1> 创建一个socket句柄
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        logger->info("Create Socket Failed!");
        return false;
    }
    logger->info("socket create successfully!");
    this->socketKey.setSocket(serverSocket);

    // <2> 设置地址重用：避免刚刚已经调用close的端口无法被绑定（之前被使用过的socket，操作系统会保持一定的TIME_WAIT时间内禁止再次绑定 ）
    int on = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
    {
        logger->info("set SO_REUSEADDR failed");
        return false;
    }

    // <3> 绑定socket句柄和地址+端口
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((u_short)nSocketPort);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        logger->info("Bind error.Port[%d]", serverAddr.sin_port);
        return false;
    }
    this->socketKey.setSocketAddr(serverAddr);


    // <4> 设置accept/recv超时:1秒
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
    logger->info("Listening on port[% d]", serverAddr.sin_port);

    // <6> 启动客户端数据处理的异步任务线程池
    this->clientThread.create(this->nThreads);

    // <7> 启动一个专门监听接入的listener线程
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, ref(*this));

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
    thread* thread = this->recvThread;    
    if (thread != nullptr)
    {
        if (thread->joinable())
        {
            thread->join();
        }
        delete this->recvThread;
        this->recvThread = nullptr;
    }

    // 重置标识
    this->setFinished(true);
    this->setExit(false);
    this->socketHandler->setExit(false);

    // 关闭服务端socket
    int serverSocket = this->socketKey.getSocket();
    if (serverSocket != -1)
    {
        ::shutdown(serverSocket, 0x02);
        ::close(serverSocket);
        this->socketKey.setSocket(-1);
    }
    
}

void FoxTcpServerSocket::setThreads(int nThreads)
{
    lock_guard<mutex> guard(this->lock);
    this->nThreads = nThreads;
}

int FoxTcpServerSocket::getThreads()
{
    lock_guard<mutex> guard(this->lock);
    return this->nThreads;
}

int FoxTcpServerSocket::getServerSocket()
{
	return this->socketKey.getSocket();
}

sockaddr_in FoxTcpServerSocket::getServerAddr()
{
    return this->socketKey.getSocketAddr();
}

bool FoxTcpServerSocket::bindSocketHandler(FoxSocketHandler* socketHandler)
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

void FoxTcpServerSocket::recvThreadFunc(FoxTcpServerSocket& socket)
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
        int hClientSocket = accept(socket.socketKey.getSocket(), (sockaddr*)&clientAddr, (socklen_t*)(&ilength));
        if (-1 == hClientSocket)
        {            
            continue;
        }

        // 接入了一个客户socket
        logger->info("try cconnect from client, address : %s, port : %d ,Socket Num : % d",
            inet_ntoa(clientAddr.sin_addr),
            clientAddr.sin_port,
            hClientSocket
        );

        // 线程池是否繁忙状态
        if (socket.clientThread.isBusy())
        {
            // 关闭这个无法继续接入的socket
            ::shutdown(hClientSocket, 0x02);
            ::close(hClientSocket);

            logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
                inet_ntoa(clientAddr.sin_addr),
                clientAddr.sin_port,
                hClientSocket
            );

            continue;
        }
        
        // 发出一个任务去处理这个客户端接入：new出来的socketKey和clientAddr，在使用完后会被FoxTcpServerRecver自动回收，所以不需要主动释放
        FoxSocketKey socketKey;
        socketKey.setSocket(hClientSocket);
        socketKey.setSocketAddr(clientAddr);        
        FoxTcpServerRecver* serverRecver = new FoxTcpServerRecver(socketKey, socket.socketHandler);
        socket.clientThread.execute(serverRecver);
    }

    // 退出线程
    logger->info("finish listenThreadFunc from server, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(socket.getServerAddr().sin_addr),
        socket.getServerAddr().sin_port,
        socket.getServerSocket());

    socket.setFinished(true);
}