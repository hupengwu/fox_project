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
    // ������һ���ͻ�socket
    logger->info("handleConnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}

void FoxTcpClientDemoHandler::handleRead(FoxTcpSocketKey& key, const char* buff, int length)
{
    // ���յ�������
    logger->info("handleRead from client, address : %s, port : %d ,Socket Num : % d,message =  %s",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket(),
        buff);


    // ������ԭ������
    key.writeSocket(buff, length);
}

void FoxTcpClientDemoHandler::handleDisconnect(FoxTcpSocketKey& key)
{
    // ������һ���ͻ�socket
    logger->info("handleDisconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}


