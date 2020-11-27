#include "FoxUdpServerSocket.h"

#include <FoxLoggerFactory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

FoxUdpServerSocket::FoxUdpServerSocket()
{
}

FoxUdpServerSocket::~FoxUdpServerSocket()
{
}

bool FoxUdpServerSocket::create(int serverPort)
{
    // <1> ����socket
    int serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> ����recv��ʱ:1��
    struct timeval timeout = { 1,0 };
    if (setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    // <3> ��socket����͵�ַ+�˿�
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((u_short)serverPort);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        logger->info("Bind error.Port[%d]", serverAddr.sin_port);
        return false;
    }
    this->socketKey.setSocketAddr(serverAddr);

    // <4> ����socket��Ϣ
    this->socketKey.setSocket(serverSocket);
    this->socketKey.setSocketAddr(serverAddr);
    this->socketKey.setInvalid(false);

    // <5> ����һ��ר���շ����߳�
    FoxUdpServerSocket* socket = this;
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, socket);

    return true;
}

int FoxUdpServerSocket::sendTo(const char* buff, int buffLen, sockaddr_in& addr_client,int addrLen)
{
    int socket = this->socketKey.getSocket();
    return ::sendto(socket, buff, buffLen, 0, (struct sockaddr*)&addr_client, addrLen);
}

void FoxUdpServerSocket::close()
{
    // ֪ͨhandler�˳���handler������Ϻ󣬹رտͻ��˵�socket
    this->socketHandler->setExit(true);

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

    // �رձ���socket
    int socket = this->socketKey.getSocket();
    if (socket != -1)
    {
        this->socketHandler->handleClosed(this->socketKey);

        ::shutdown(socket, 0x02);
        ::close(socket);
        this->socketKey.setSocket(-1);
    }
}

void FoxUdpServerSocket::recvFunc(FoxSocket* socket)
{
    FoxUdpServerSocket* serverSocket = (FoxUdpServerSocket*)socket;
    FoxSocketHandler& handler = *serverSocket->socketHandler;
    FoxSocketKey& serverKey = serverSocket->socketKey;

    struct sockaddr_in      addr_client;
    int                     addr_len;


    // <1> ���յ��ͻ��˷���������Ϣ       
    int length = recvfrom(serverKey.getSocket(), recvBuff, sizeof(recvBuff), 0, (struct sockaddr*)&addr_client, (socklen_t*)&addr_len);
    if (length < 0)
    {
        return;
    }

    // <2> ���յ��˿ͻ��˷��͹��������ݣ����ڻ����0��
    if (length >= 0)
    {
        handler.handleReadFrom(serverKey, recvBuff, length, addr_client, addr_len);
        return;
    }
}
