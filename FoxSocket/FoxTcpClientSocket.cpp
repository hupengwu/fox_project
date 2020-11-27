#include "FoxTcpClientSocket.h"

#include <FoxLoggerFactory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

FoxTcpClientSocket::FoxTcpClientSocket()
{
}

FoxTcpClientSocket::~FoxTcpClientSocket()
{
}

bool FoxTcpClientSocket::connect(const char* remoteIP, int remotePort)
{
    // <1> ����socket
    int localSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (localSocket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> ����recv��ʱ:1��
    struct timeval timeout = { 1,0 };
    if (setsockopt(localSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    // ��ʼ����ַ�ṹ
    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons((u_short)remotePort);
    remoteAddr.sin_addr.s_addr = inet_addr(remoteIP);

    // <3> ���ӷ����
    if (::connect(localSocket, (struct sockaddr*)&remoteAddr, sizeof(struct sockaddr)) < 0)
    {
        ::close(localSocket);
        logger->info("Connect error.IP[%s], port[%d]", remoteIP, remoteAddr.sin_port);
        return false;
    }
    logger->info("Connect to IP[%s], port[%d]", remoteIP, remoteAddr.sin_port);

    // <4> ����socket��Ϣ
    this->socketKey.setSocket(localSocket);
    this->socketKey.setSocketAddr(remoteAddr);
    this->socketKey.setInvalid(false);

    // <5> ֪ͨ���ӷ���˳ɹ�
    this->socketHandler->handleConnect(this->socketKey);

    // <6> ����һ��ר���ַ����߳�
    FoxTcpClientSocket* clientSocket = this;
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, clientSocket);

    return true;
}

int FoxTcpClientSocket::send(const char* buff, int length)
{
    int localSocket = this->socketKey.getSocket();
    return ::send(localSocket, buff, length,0);
}

void FoxTcpClientSocket::close()
{
    // ֪ͨhandler�˳���handler������Ϻ󣬹رտͻ��˵�socket
    this->socketHandler->setExit(true);

    // ֪ͨ�����߳��˳�
    this->setExit(true);

    // ��飺ȫ���߳��Ƿ����н���
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(100));
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

    // �رձ���socket
    int localSocket = this->socketKey.getSocket();
    if (localSocket != -1)
    {
        this->socketHandler->handleDisconnect(this->socketKey);
        this->socketHandler->handleClosed(this->socketKey);

        ::shutdown(localSocket, 0x02);
        ::close(localSocket);
        this->socketKey.setSocket(-1);
    }
}

void FoxTcpClientSocket::recvFunc(FoxSocket* socket)
{
    FoxTcpClientSocket* clientSocket = (FoxTcpClientSocket*)socket;
    FoxSocketHandler& handler        = *clientSocket->socketHandler;
    FoxSocketKey& localKey           = clientSocket->socketKey;

    // <1> ���յ�����˷���������Ϣ
    int length = ::recv(localKey.getSocket(), recvBuff, sizeof(recvBuff), 0);
    if (-1 == length)
    {
        return;
    }

    // <2> ���յ��˷���˷��͹��������ݣ�����0��
    if (length > 0)
    {
        handler.handleRead(localKey, recvBuff, length);
        return;
    }

    // <2> ���յ�����˶Ͽ�����Ϣ������0������ �ͻ��������Ͽ��ÿͻ������� ���� �ͻ���socketͨ��handler֪ͨ�������˳�����
    if ((length == 0) || localKey.getInvalid() || handler.getExit())
    {
        logger->info("disconnect from client, server : %s, port : %d ,Socket Num : % d",
            inet_ntoa(localKey.getSocketAddr().sin_addr),
            localKey.getSocketAddr().sin_port,
            socket);

        // �رձ���socket
        int localSocket = localKey.getSocket();
        if (localSocket != -1)
        {
            this->socketHandler->handleDisconnect(this->socketKey);
            this->socketHandler->handleClosed(this->socketKey);

            ::shutdown(localSocket, 0x02);
            ::close(localSocket);
            localKey.setSocket(-1);
        }

        return;
    }
}

