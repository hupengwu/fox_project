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
    // <1> 创建socket
    int socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> 设置recv超时:1秒
    struct timeval timeout = { 1,0 };
    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    // 初始化地址结构
    struct sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port = htons((u_short)serverPort);
    socketAddr.sin_addr.s_addr = inet_addr(serverIP);

    // <3> 连接服务端
    if (::connect(socket, (struct sockaddr*)&socketAddr, sizeof(struct sockaddr)) < 0)
    {
        ::close(socket);
        logger->info("Connect error.IP[%s], port[%d]", serverIP, socketAddr.sin_port);
        return false;
    }
    logger->info("Connect to IP[%s], port[%d]", serverIP, socketAddr.sin_port);

    // <4> 保存socket信息
    this->socketKey.setSocket(socket);
    this->socketKey.setSocketAddr(socketAddr);
    this->socketKey.setInvalid(false);

    // <5> 通知连接服务端成功
    this->socketHandler->handleConnect(this->socketKey);

    // <6> 启动一个专门手法的线程
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
    // 通知handler退出：handler处理完毕后，关闭客户端的socket
    this->socketHandler->setExit(true);

    // 通知监听线程退出
    this->setExit(true);

    // 检查：全体线程是否运行结束
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(100));
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

    // <1> 接收到服务端发过来的消息
    int length = ::recv(key.getSocket(), recvBuff, sizeof(recvBuff), 0);
    if (-1 == length)
    {
        return;
    }

    // <2> 接收到了服务端发送过来的数据（大于0）
    if (length > 0)
    {
        handler.handleRead(key, recvBuff, length);
        return;
    }

    // <2> 接收到服务端断开的消息（等于0）或者 客户端主动断开该客户端连接 或者 客户端socket通过handler通知过来的退出请求
    if ((length == 0) || key.getInvalid() || handler.getExit())
    {
        logger->info("disconnect from client, server : %s, port : %d ,Socket Num : % d",
            inet_ntoa(key.getSocketAddr().sin_addr),
            key.getSocketAddr().sin_port,
            socket);

        // 关闭本地socket
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

