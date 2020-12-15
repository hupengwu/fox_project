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
    // <1> ����һ��socket���
    int localSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (localSocket == -1)
    {
        logger->info("Create Socket Failed!");
        return false;
    }
    logger->info("socket create successfully!");    

    // <2> ���õ�ַ���ã�����ո��Ѿ�����close�Ķ˿��޷����󶨣�֮ǰ��ʹ�ù���socket������ϵͳ�ᱣ��һ����TIME_WAITʱ���ڽ�ֹ�ٴΰ� ��
    int on = 1;
    if (::setsockopt(localSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
    {
        ::close(localSocket);
        logger->info("set SO_REUSEADDR failed");
        return false;
    }

    // <3> ��socket����͵�ַ+�˿�
    sockaddr_in localAddr;
    bzero(&localAddr, sizeof(struct sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons((u_short)localPort);
    if (::bind(localSocket, (struct sockaddr*)&localAddr, sizeof(struct sockaddr)) < 0)
    {
        ::close(localSocket);
        logger->info("Bind error.Port[%d]", localAddr.sin_port);
        return false;
    }
    this->socketKey.setSocketAddr(localAddr);


    // <4> ����accept/recv��ʱ:1��
    struct timeval timeout = { 1,0 };
    if (::setsockopt(localSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        ::close(localSocket);
        logger->info("set recv timeout failed");
        return false;
    }

    // <5> serverSocket���ڼ���
    if (::listen(localSocket, 20))
    {
        ::close(localSocket);
        logger->info("Listen error!");
        return false;
    }
    logger->info("Listening on port[% d]", localPort);

    // ����localSocket
    this->socketKey.setSocket(localSocket);

    // <6> �����ͻ������ݴ�����첽�����̳߳�
    this->clientThread.create(this->nThreads);
    
    // <7> ����һ��ר�ż��������listener�߳�
    this->createThread();
    
    return true;
}

void FoxTcpServerSocket::close()
{
    // ֪ͨhandler�˳���handler������Ϻ󣬹رտͻ��˵�socket
    this->socketHandler->setExit(true);

    this->closeThread();    
    
    this->socketHandler->setExit(false);

    // �رշ����socket
    int localSocket = this->socketKey.getSocket();
    if (localSocket != -1)
    {
        this->socketHandler->handleClosed(this->socketKey);

        ::shutdown(localSocket, 0x02);
        ::close(localSocket);
        this->socketKey.setSocket(-1);
    } 
}

bool FoxTcpServerSocket::setRevTimeOut(timeval& timeout)
{
    int localSocket = this->socketKey.getSocket();
    if (localSocket == -1)
    {
        return false;
    }

    // ����accept/recv��ʱ
     if (::setsockopt(localSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    return true;
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

void FoxTcpServerSocket::recvFunc(STLThreadObject* socket)
{  
    FoxTcpServerSocket* localSocket = (FoxTcpServerSocket*)socket;
    FoxSocketKey& localKey = localSocket->socketKey;


    //����ͻ��˵�socket��ַ�ṹclientAddr
    sockaddr_in remoteAddr;
    int remoteAddrLen = sizeof(remoteAddr);

    // �ȴ��ͻ���socket�Ľ���
    int remoteSocket = ::accept(localKey.getSocket(), (sockaddr*)&remoteAddr, (socklen_t*)(&remoteAddrLen));
    if (-1 == remoteSocket)
    {
        return;
    }

    // ������һ���ͻ�socket
    logger->info("try cconnect from client, address : %s, port : %d ,Socket Num : % d",
        ::inet_ntoa(remoteAddr.sin_addr),
        remoteAddr.sin_port,
        remoteSocket
    );

    // �̳߳��Ƿ�æ״̬
    if (localSocket->clientThread.isBusy())
    {
        // �ر�����޷����������socket
        ::shutdown(remoteSocket, 0x02);
        ::close(remoteSocket);

        logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
            inet_ntoa(remoteAddr.sin_addr),
            remoteAddr.sin_port,
            remoteSocket
        );

        return;
    }

    // ����һ������ȥ��������ͻ��˽��룺new������socketKey��clientAddr����ʹ�����ᱻFoxTcpServerRecver�Զ����գ����Բ���Ҫ�����ͷ�
    FoxSocketKey remoteKey;
    remoteKey.setSocket(remoteSocket);
    remoteKey.setSocketAddr(remoteAddr);
    FoxTcpServerRecver* remoteRecver = new FoxTcpServerRecver(remoteKey, localSocket->socketHandler);
    localSocket->clientThread.execute(remoteRecver);
    
}
