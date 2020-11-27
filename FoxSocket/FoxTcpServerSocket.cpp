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

FoxTcpServerSocket::FoxTcpServerSocket()
{
    this->nThreads = 5;
}

FoxTcpServerSocket::~FoxTcpServerSocket()
{
}

bool FoxTcpServerSocket::create(int localPort)
{
    // <1> 创建一个socket句柄
    int localSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (localSocket == -1)
    {
        logger->info("Create Socket Failed!");
        return false;
    }
    logger->info("socket create successfully!");
    this->socketKey.setSocket(localSocket);

    // <2> 设置地址重用：避免刚刚已经调用close的端口无法被绑定（之前被使用过的socket，操作系统会保持一定的TIME_WAIT时间内禁止再次绑定 ）
    int on = 1;
    if (setsockopt(localSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
    {
        logger->info("set SO_REUSEADDR failed");
        return false;
    }

    // <3> 绑定socket句柄和地址+端口
    sockaddr_in localAddr;
    bzero(&localAddr, sizeof(struct sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons((u_short)localPort);
    if (bind(localSocket, (struct sockaddr*)&localAddr, sizeof(struct sockaddr)) < 0)
    {
        logger->info("Bind error.Port[%d]", localAddr.sin_port);
        return false;
    }
    this->socketKey.setSocketAddr(localAddr);


    // <4> 设置accept/recv超时:1秒
    struct timeval timeout = { 1,0 };
    if (setsockopt(localSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set accept timeout failed");
        return false;
    }

    // <5> serverSocket用于监听
    if (listen(localSocket, 20))
    {
        logger->info("Listen error!");
        return false;
    }
    logger->info("Listening on port[% d]", localAddr.sin_port);

    // <6> 启动客户端数据处理的异步任务线程池
    this->clientThread.create(this->nThreads);
    
    // <7> 启动一个专门监听接入的listener线程
    FoxTcpServerSocket* socket = this;
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, socket);
    
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
    int localSocket = this->socketKey.getSocket();
    if (localSocket != -1)
    {
        this->socketHandler->handleClosed(this->socketKey);

        ::shutdown(localSocket, 0x02);
        ::close(localSocket);
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

void FoxTcpServerSocket::recvFunc(FoxSocket* socket)
{  
    FoxTcpServerSocket* localSocket = (FoxTcpServerSocket*)socket;
    FoxSocketKey& localKey = localSocket->socketKey;


    //定义客户端的socket地址结构clientAddr
    sockaddr_in remoteAddr;
    int remoteAddrLen = sizeof(remoteAddr);

    // 等待客户端socket的接入
    int remoteSocket = accept(localKey.getSocket(), (sockaddr*)&remoteAddr, (socklen_t*)(&remoteAddrLen));
    if (-1 == remoteSocket)
    {
        return;
    }

    // 接入了一个客户socket
    logger->info("try cconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(remoteAddr.sin_addr),
        remoteAddr.sin_port,
        remoteSocket
    );

    // 线程池是否繁忙状态
    if (localSocket->clientThread.isBusy())
    {
        // 关闭这个无法继续接入的socket
        ::shutdown(remoteSocket, 0x02);
        ::close(remoteSocket);

        logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
            inet_ntoa(remoteAddr.sin_addr),
            remoteAddr.sin_port,
            remoteSocket
        );

        return;
    }

    // 发出一个任务去处理这个客户端接入：new出来的socketKey和clientAddr，在使用完后会被FoxTcpServerRecver自动回收，所以不需要主动释放
    FoxSocketKey remoteKey;
    remoteKey.setSocket(remoteSocket);
    remoteKey.setSocketAddr(remoteAddr);
    FoxTcpServerRecver* remoteRecver = new FoxTcpServerRecver(remoteKey, localSocket->socketHandler);
    localSocket->clientThread.execute(remoteRecver);
    
}
