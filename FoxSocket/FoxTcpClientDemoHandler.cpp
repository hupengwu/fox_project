#include "FoxTcpClientDemoHandler.h"

#include <FoxLoggerFactory.h>
#include <STLStringUtils.h>
#include <arpa/inet.h>

ILogger* FoxTcpClientDemoHandler::logger = FoxLoggerFactory::getLogger();

FoxTcpClientDemoHandler::FoxTcpClientDemoHandler()
{
}

FoxTcpClientDemoHandler::~FoxTcpClientDemoHandler()
{
}

void FoxTcpClientDemoHandler::handleConnect(FoxTcpSocketKey& key)
{
    // 接入了一个客户socket
    logger->info("handleConnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}

void FoxTcpClientDemoHandler::handleRead(FoxTcpSocketKey& key, const char* buff, int length)
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

void FoxTcpClientDemoHandler::handleDisconnect(FoxTcpSocketKey& key)
{
    // 接入了一个客户socket
    logger->info("handleDisconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}


