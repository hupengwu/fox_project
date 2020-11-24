#include "FoxTcpServerRecver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <FoxLoggerFactory.h>
#include <STLStringUtils.h>

#include "FoxTcpSocketKey.h"

ILogger* FoxTcpServerRecver::logger = FoxLoggerFactory::getLogger();

constexpr auto BUFF_SIZE_MAX = 16*1024;

void FoxTcpServerRecver::run()
{
    char recvBuff[BUFF_SIZE_MAX];

    FoxTcpSocketHandler& handler    = *this->socketHandler;
    FoxTcpSocketKey& key            = this->socketKey;
    int socket                      = key.getSocket();

    // <1> 客户端连接进来
    logger->info("connect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        socket);
    handler.handleConnect(key);

    while (true)
    {
        // 接收客户端发送来的信息到buffer中
        int length = ::recv(socket, recvBuff, BUFF_SIZE_MAX, 0);

        // <2> 接收到数据（大于0）
        if (length > 0)
        {
            handler.handleRead(key, recvBuff, length);
        }

        // <3> 接收到客户端断开的消息（等于0）或者 服务端主动断开该客户端连接 或者 服务端socket通过handler通知过来的退出请求
        if ((length == 0) || key.getInvalid() || handler.getExit())
        {
            logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
                inet_ntoa(key.getSocketAddr().sin_addr),
                key.getSocketAddr().sin_port,
                socket);

            handler.handleDisconnect(key);
            break;
        }
    }

    // 关闭socket
    ::shutdown(socket, 0x02);
    ::close(socket);

    return;
}

FoxTcpServerRecver::FoxTcpServerRecver(FoxTcpSocketKey& socketKey, FoxTcpSocketHandler* socketHandler)
{
    this->socketKey = socketKey;
    this->socketHandler = socketHandler;
}

FoxTcpServerRecver::~FoxTcpServerRecver()
{
}

