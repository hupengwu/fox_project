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
    // <1> ����һ��socket���
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        logger->info("Create Socket Failed!");
        return false;
    }
    logger->info("socket create successfully!");
    this->socketKey.setSocket(serverSocket);

    // <2> ���õ�ַ���ã�����ո��Ѿ�����close�Ķ˿��޷����󶨣�֮ǰ��ʹ�ù���socket������ϵͳ�ᱣ��һ����TIME_WAITʱ���ڽ�ֹ�ٴΰ� ��
    int on = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
    {
        logger->info("set SO_REUSEADDR failed");
        return false;
    }

    // <3> ��socket����͵�ַ+�˿�
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


    // <4> ����accept/recv��ʱ:1��
    struct timeval timeout = { 1,0 };
    if (setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set accept timeout failed");
        return false;
    }

    // <5> serverSocket���ڼ���
    if (listen(serverSocket, 20))
    {
        logger->info("Listen error!");
        return false;
    }
    logger->info("Listening on port[% d]", serverAddr.sin_port);

    // <6> �����ͻ������ݴ�����첽�����̳߳�
    this->clientThread.create(this->nThreads);

    // <7> ����һ��ר�ż��������listener�߳�
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, ref(*this));

    return true;
}

void FoxTcpServerSocket::close()
{
    // ֪ͨhandler�˳���handler������Ϻ󣬹رտͻ��˵�socket
    this->socketHandler->setExit(true);
    
    // �رմ���ͻ��˵��̳߳�
	this->clientThread.close();

    // ֪ͨ�����߳��˳�
    this->setExit(true);

    // ��飺ȫ���߳��Ƿ����н���
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    // �����߳�
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

    // ���ñ�ʶ
    this->setFinished(true);
    this->setExit(false);
    this->socketHandler->setExit(false);

    // �رշ����socket
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
        // ��飺�˳��̱߳��
        if (socket.getExit())
        {
            break;
        }

        //����ͻ��˵�socket��ַ�ṹclientAddr
        sockaddr_in clientAddr;
        int ilength = sizeof(clientAddr);

        // �ȴ��ͻ���socket�Ľ���
        int hClientSocket = accept(socket.socketKey.getSocket(), (sockaddr*)&clientAddr, (socklen_t*)(&ilength));
        if (-1 == hClientSocket)
        {            
            continue;
        }

        // ������һ���ͻ�socket
        logger->info("try cconnect from client, address : %s, port : %d ,Socket Num : % d",
            inet_ntoa(clientAddr.sin_addr),
            clientAddr.sin_port,
            hClientSocket
        );

        // �̳߳��Ƿ�æ״̬
        if (socket.clientThread.isBusy())
        {
            // �ر�����޷����������socket
            ::shutdown(hClientSocket, 0x02);
            ::close(hClientSocket);

            logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
                inet_ntoa(clientAddr.sin_addr),
                clientAddr.sin_port,
                hClientSocket
            );

            continue;
        }
        
        // ����һ������ȥ��������ͻ��˽��룺new������socketKey��clientAddr����ʹ�����ᱻFoxTcpServerRecver�Զ����գ����Բ���Ҫ�����ͷ�
        FoxSocketKey socketKey;
        socketKey.setSocket(hClientSocket);
        socketKey.setSocketAddr(clientAddr);        
        FoxTcpServerRecver* serverRecver = new FoxTcpServerRecver(socketKey, socket.socketHandler);
        socket.clientThread.execute(serverRecver);
    }

    // �˳��߳�
    logger->info("finish listenThreadFunc from server, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(socket.getServerAddr().sin_addr),
        socket.getServerAddr().sin_port,
        socket.getServerSocket());

    socket.setFinished(true);
}