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
    // <1> 创建socket
    int localSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (localSocket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> 设置recv超时:1秒
    struct timeval timeout = { 1,0 };
    if (::setsockopt(localSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    // 初始化地址结构
    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons((u_short)remotePort);
    remoteAddr.sin_addr.s_addr = inet_addr(remoteIP);

    // <3> 连接服务端
    if (::connect(localSocket, (struct sockaddr*)&remoteAddr, sizeof(struct sockaddr)) < 0)
    {
        ::close(localSocket);
        logger->info("Connect error.IP[%s], port[%d]", remoteIP, remoteAddr.sin_port);
        return false;
    }
    logger->info("Connect to IP[%s], port[%d]", remoteIP, remoteAddr.sin_port);

    // <4> 保存socket信息
    this->socketKey.setSocket(localSocket);
    this->socketKey.setSocketAddr(remoteAddr);
    this->socketKey.setInvalid(false);

    // <5> 通知连接服务端成功
    this->socketHandler->handleConnect(this->socketKey);

    // <6> 启动一个专门手法的线程
    this->createThread();

    return true;
}

int FoxTcpClientSocket::send(const char* buff, int length)
{
    int localSocket = this->socketKey.getSocket();
    return ::send(localSocket, buff, length,0);
}

void FoxTcpClientSocket::close()
{
    // 通知handler退出：handler处理完毕后，关闭客户端的socket
    this->socketHandler->setExit(true);

    // 关闭线程
    this->closeThread();

    this->socketHandler->setExit(false);

    // 关闭本地socket
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

void FoxTcpClientSocket::recvFunc(STLThreadObject* socket)
{
    FoxTcpClientSocket* clientSocket = (FoxTcpClientSocket*)socket;
    FoxSocketHandler& handler        = *clientSocket->socketHandler;
    FoxSocketKey& localKey           = clientSocket->socketKey;
    int recvSize                     = sizeof(recvBuff);

    // <1> 接收到服务端发过来的消息
    int length = ::recv(localKey.getSocket(), recvBuff, recvSize, 0);
    if (-1 == length)
    {
        int err = errno;
        if (err == EAGAIN)
        {
            // 重试：连续到了超时都没有接收到数据,为正常状况
            this->socketHandler->handleNoRead(localKey);
        }

        return;
    }

    // <2> 接收到了服务端发送过来的数据（大于0）
    if (length > 0)
    {
        handler.handleRead(localKey, recvBuff, length);

        if (length < recvSize)
        {
            // 服务端发送过来的一组数据，已经通过多次recv动作取完了
            this->socketHandler->handleHasRead(localKey);
        }
        return;
    }

    // <2> 接收到服务端断开的消息（等于0）或者 客户端主动断开该客户端连接 或者 客户端socket通过handler通知过来的退出请求
    if ((length == 0) || localKey.getInvalid() || handler.getExit())
    {
        logger->info("disconnect from client, server : %s, port : %d ,Socket Num : % d",
            inet_ntoa(localKey.getSocketAddr().sin_addr),
            localKey.getSocketAddr().sin_port,
            socket);

        // 关闭本地socket
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

