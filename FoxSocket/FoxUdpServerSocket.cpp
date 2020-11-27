#include "FoxUdpServerSocket.h"

#include <FoxLoggerFactory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

ILogger* FoxUdpServerSocket::logger = FoxLoggerFactory::getLogger();

FoxUdpServerSocket::FoxUdpServerSocket()
{
    this->isExit = false;
    this->bFinished = true;
    this->socketHandler = nullptr;
    this->recvThread = nullptr;

    this->socketHandler = new FoxSocketHandler();
}

FoxUdpServerSocket::~FoxUdpServerSocket()
{
    delete this->socketHandler;
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
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, ref(*this));

    return true;
}

int FoxUdpServerSocket::send(const char* buff, int length)
{
    int socket = this->socketKey.getSocket();
    return ::send(socket, buff, length, 0);
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
        ::shutdown(socket, 0x02);
        ::close(socket);
        this->socketKey.setSocket(-1);
    }

    this->socketHandler->handleDisconnect(this->socketKey);
}

bool FoxUdpServerSocket::bindSocketHandler(FoxSocketHandler* socketHandler)
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

void FoxUdpServerSocket::setFinished(bool finished)
{
    lock_guard<mutex> guard(this->lock);
    this->bFinished = finished;
}

bool FoxUdpServerSocket::getFinished()
{
    lock_guard<mutex> guard(this->lock);
    return this->bFinished;
}

void FoxUdpServerSocket::setExit(bool isExit)
{
    lock_guard<mutex> guard(this->lock);
    this->isExit = isExit;
}

bool FoxUdpServerSocket::getExit()
{
    lock_guard<mutex> guard(this->lock);
    return this->isExit;
}

constexpr auto BUFF_SIZE_MAX = 16 * 1024;

void FoxUdpServerSocket::recvThreadFunc(FoxUdpServerSocket& socket)
{
    char recvBuff[BUFF_SIZE_MAX];

    FoxSocketHandler& handler = *socket.socketHandler;
    FoxSocketKey& serverKey = socket.socketKey;
    int serverSocket = serverKey.getSocket();

    struct sockaddr_in      addr_client;
    int                     addr_len;

    while (true)
    {
        // ��飺�˳��̱߳��
        if (socket.getExit())
        {
            break;
        }


        // <1> ���յ��ͻ��˷���������Ϣ       
        int length = recvfrom(serverSocket, recvBuff, sizeof(recvBuff), 0, (struct sockaddr*)&addr_client, (socklen_t*)&addr_len);
        if (length < 0)
        {
            continue;
        }

        // <2> ���յ��˿ͻ��˷��͹��������ݣ����ڻ����0��
        if (length >= 0)
        {
            FoxSocketKey socketKey;
            socketKey.setSocket(serverSocket);
            socketKey.setSocketAddr(addr_client);

            handler.handleRead(socketKey, recvBuff, length);
            continue;
        }

        // <3> ����������ر� ���� �����socketͨ��handler֪ͨ�������˳�����
        if (serverKey.getInvalid() || handler.getExit())
        {
            logger->info("close server : %s, port : %d ,Socket Num : % d",
                inet_ntoa(serverKey.getSocketAddr().sin_addr),
                serverKey.getSocketAddr().sin_port,
                serverSocket
            );
            break;
        }

    }

    // �˳��߳�
    logger->info("finish recvThreadFunc from server, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(serverKey.getSocketAddr().sin_addr),
        serverKey.getSocketAddr().sin_port,
        serverKey.getSocket()
    );

    socket.setFinished(true);
}
