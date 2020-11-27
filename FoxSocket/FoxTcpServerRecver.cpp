#include "FoxTcpServerRecver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <FoxLoggerFactory.h>
#include <STLStringUtils.h>

#include "FoxSocketKey.h"

ILogger* FoxTcpServerRecver::logger = FoxLoggerFactory::getLogger();

void FoxTcpServerRecver::run()
{
    FoxSocketHandler& handler    = *this->socketHandler;
    FoxSocketKey& key            = this->socketKey;
    int clientSocket             = key.getSocket();

    // <1> 客户端连接进来
    logger->info("connect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        clientSocket);
    handler.handleConnect(key);

    while (true)
    {
        // <2> 接收客户端发送来的信息到buffer中
        int length = ::recv(clientSocket, recvBuff, sizeof(recvBuff), 0);

        // <3> 接收到数据（大于0）
        if (length > 0)
        {
            handler.handleRead(key, recvBuff, length);
        }

        // <4> 接收到客户端断开的消息（等于0）或者 服务端主动断开该客户端连接 或者 服务端socket通过handler通知过来的退出请求
        if ((length <= 0) || key.getInvalid() || handler.getExit())
        {            
            break;
        }
    }

    // 通知客户端连接断开
    logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        clientSocket);
    handler.handleDisconnect(key);

    // 关闭socket
    ::shutdown(clientSocket, 0x02);
    ::close(clientSocket);
    key.setSocket(-1);

    return;
}

FoxTcpServerRecver::FoxTcpServerRecver(FoxSocketKey& socketKey, FoxSocketHandler* socketHandler)
{
    this->socketKey = socketKey;
    this->socketHandler = socketHandler;
}

FoxTcpServerRecver::~FoxTcpServerRecver()
{
}

