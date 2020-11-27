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

bool FoxTcpClientSocket::connect(const char* serverIP, int serverPort)
{
    // <1> ����socket
    int socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> ����recv��ʱ:1��
    struct timeval timeout = { 1,0 };
    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    // ��ʼ����ַ�ṹ
    struct sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port = htons((u_short)serverPort);
    socketAddr.sin_addr.s_addr = inet_addr(serverIP);

    // <3> ���ӷ����
    if (::connect(socket, (struct sockaddr*)&socketAddr, sizeof(struct sockaddr)) < 0)
    {
        ::close(socket);
        logger->info("Connect error.IP[%s], port[%d]", serverIP, socketAddr.sin_port);
        return false;
    }
    logger->info("Connect to IP[%s], port[%d]", serverIP, socketAddr.sin_port);

    // <4> ����socket��Ϣ
    this->socketKey.setSocket(socket);
    this->socketKey.setSocketAddr(socketAddr);
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
    int socket = this->socketKey.getSocket();
    return ::send(socket, buff, length,0);
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
    int socket = this->socketKey.getSocket();
    if (socket != -1)
    {
        this->socketHandler->handleDisconnect(this->socketKey);
        this->socketHandler->handleClosed(this->socketKey);

        ::shutdown(socket, 0x02);
        ::close(socket);
        this->socketKey.setSocket(-1);
    }
}

void FoxTcpClientSocket::recvFunc(FoxSocket* socket)
{
    FoxTcpClientSocket* clientSocket = (FoxTcpClientSocket*)socket;
    FoxSocketHandler& handler    = *clientSocket->socketHandler;
    FoxSocketKey& key            = clientSocket->socketKey;

    // <1> ���յ�����˷���������Ϣ
    int length = ::recv(key.getSocket(), recvBuff, sizeof(recvBuff), 0);
    if (-1 == length)
    {
        return;
    }

    // <2> ���յ��˷���˷��͹��������ݣ�����0��
    if (length > 0)
    {
        handler.handleRead(key, recvBuff, length);
        return;
    }

    // <2> ���յ�����˶Ͽ�����Ϣ������0������ �ͻ��������Ͽ��ÿͻ������� ���� �ͻ���socketͨ��handler֪ͨ�������˳�����
    if ((length == 0) || key.getInvalid() || handler.getExit())
    {
        logger->info("disconnect from client, server : %s, port : %d ,Socket Num : % d",
            inet_ntoa(key.getSocketAddr().sin_addr),
            key.getSocketAddr().sin_port,
            socket);

        // �رձ���socket
        int socket = this->socketKey.getSocket();
        if (socket != -1)
        {
            this->socketHandler->handleDisconnect(this->socketKey);
            this->socketHandler->handleClosed(this->socketKey);

            ::shutdown(socket, 0x02);
            ::close(socket);
            this->socketKey.setSocket(-1);
        }

        return;
    }
}

