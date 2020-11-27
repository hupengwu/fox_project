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

void FoxTcpServerDemoHandler::handleConnect(FoxSocketKey& key)
{
    // 接入了一个客户socket
    logger->info("handleConnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}

void FoxTcpServerDemoHandler::handleRead(FoxSocketKey& key, const char* buff, int length)
{
    // 接收到的数据
    logger->info("handleRead from client, address : %s, port : %d ,Socket Num : % d,message =  %s",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket(),
        buff);


    // 将数据原样返回
    key.writeSocket(buff, length);
}

void FoxTcpServerDemoHandler::handleDisconnect(FoxSocketKey& key)
{
    // 接入了一个客户socket
    logger->info("handleDisconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}
