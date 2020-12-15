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
    FoxSocketKey& remoteKey      = this->socketKey;
    int remoteSocket             = remoteKey.getSocket();
    int recvSize                 = sizeof(recvBuff);

    // <1> 客户端连接进来
    logger->info("connect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(remoteKey.getSocketAddr().sin_addr),
        remoteKey.getSocketAddr().sin_port,
        remoteSocket);
    handler.handleConnect(remoteKey);

    while (true)
    {
        // <2> 接收客户端发送来的信息到buffer中
        int length = ::recv(remoteSocket, recvBuff, recvSize, 0);

        // <3> length == -1时
        if (length == -1)
        {         
            int err = errno;
            if (err == EAGAIN)
            {
                // 重试：连续到了超时都没有接收到数据,为正常状况
                this->socketHandler->handleNoRead(remoteKey);
                continue;
            }
            else if (err == EINTR)
            {
                // 中断：发生系统调用，为正常状况
                continue;
            }
            else
            {
                // 出错状况：其他出错码，比如客户端主动断开，socket被关闭等异常状况
                break;
            }
        }

        // <4> 接收到数据（大于0）
        if (length > 0)
        {
            handler.handleRead(remoteKey, recvBuff, length);
            if (length < recvSize)
            {
                // 客户端发送过来的一组数据，已经通过多次recv动作取完了
                this->socketHandler->handleHasRead(remoteKey);
            }
            
        }       

        // <5> 接收到客户端断开的消息（等于0）或者 服务端主动断开该客户端连接 或者 服务端socket通过handler通知过来的退出请求
        if (length == 0 || remoteKey.getInvalid() || handler.getExit())
        {      
            break;
        }
    }

    // 通知客户端连接断开
    logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(remoteKey.getSocketAddr().sin_addr),
        remoteKey.getSocketAddr().sin_port,
        remoteSocket);
    handler.handleDisconnect(remoteKey);

    // 关闭socket
    ::shutdown(remoteSocket, 0x02);
    ::close(remoteSocket);
    remoteKey.setSocket(-1);

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

