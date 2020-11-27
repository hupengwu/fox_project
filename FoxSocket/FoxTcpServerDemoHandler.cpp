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
    // ������һ���ͻ�socket
    logger->info("handleConnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}

void FoxTcpServerDemoHandler::handleRead(FoxSocketKey& key, const char* buff, int length)
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

void FoxTcpServerDemoHandler::handleDisconnect(FoxSocketKey& key)
{
    // ������һ���ͻ�socket
    logger->info("handleDisconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket());
}
