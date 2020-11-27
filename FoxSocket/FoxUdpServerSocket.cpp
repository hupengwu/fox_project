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
    // <1> 创建socket
    int serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> 设置recv超时:1秒
    struct timeval timeout = { 1,0 };
    if (setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    // <3> 绑定socket句柄和地址+端口
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

    // <4> 保存socket信息
    this->socketKey.setSocket(serverSocket);
    this->socketKey.setSocketAddr(serverAddr);
    this->socketKey.setInvalid(false);

    // <5> 启动一个专门收发的线程
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
    // 通知handler退出：handler处理完毕后，关闭客户端的socket
    this->socketHandler->setExit(true);

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

    // 关闭本地socket
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
        // 检查：退出线程标记
        if (socket.getExit())
        {
            break;
        }


        // <1> 接收到客户端发过来的消息       
        int length = recvfrom(serverSocket, recvBuff, sizeof(recvBuff), 0, (struct sockaddr*)&addr_client, (socklen_t*)&addr_len);
        if (length < 0)
        {
            continue;
        }

        // <2> 接收到了客户端发送过来的数据（大于或等于0）
        if (length >= 0)
        {
            FoxSocketKey socketKey;
            socketKey.setSocket(serverSocket);
            socketKey.setSocketAddr(addr_client);

            handler.handleRead(socketKey, recvBuff, length);
            continue;
        }

        // <3> 服务端主动关闭 或者 服务端socket通过handler通知过来的退出请求
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

    // 退出线程
    logger->info("finish recvThreadFunc from server, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(serverKey.getSocketAddr().sin_addr),
        serverKey.getSocketAddr().sin_port,
        serverKey.getSocket()
    );

    socket.setFinished(true);
}
