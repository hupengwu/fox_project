#include "FoxTcpServerDemoHandler.h"

#include <FoxLoggerFactory.h>
#include <STLStringUtils.h>
#include <arpa/inet.h>

ILogger* FoxTcpServerDemoHandler::logger = FoxLoggerFactory::getLogger();

FoxTcpServerDemoHandler::FoxTcpServerDemoHandler()
{
}

FoxTcpServerDemoHandler::~FoxTcpServerDemoHandler()
{
}

void FoxTcpServerDemoHandler::handleConnect(FoxTcpSocketKey& key)
{
    // 接入了一个客户socket
    string message = STLStringUtils::snprintf("handleConnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket()
    );
    logger->info(message);
}

void FoxTcpServerDemoHandler::handleRead(FoxTcpSocketKey& key, const char* buff, int length)
{
    // 接收到的数据
    string message = STLStringUtils::snprintf("handleRead from client, address : %s, port : %d ,Socket Num : % d,message =  %s",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket(),
        buff
    );
    logger->info(message);


    // 将数据原样返回
    key.writeSocket(buff, length);
}

void FoxTcpServerDemoHandler::handleDisconnect(FoxTcpSocketKey& key)
{
    // 接入了一个客户socket
    string message = STLStringUtils::snprintf("handleDisconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket()
    );
    logger->info(message);
}
