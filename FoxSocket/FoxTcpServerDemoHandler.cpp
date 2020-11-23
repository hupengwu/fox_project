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
    // ������һ���ͻ�socket
    string message = STLStringUtils::snprintf("handleConnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket()
    );
    logger->info(message);
}

void FoxTcpServerDemoHandler::handleRead(FoxTcpSocketKey& key, const char* buff, int length)
{
    // ���յ�������
    string message = STLStringUtils::snprintf("handleRead from client, address : %s, port : %d ,Socket Num : % d,message =  %s",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket(),
        buff
    );
    logger->info(message);


    // ������ԭ������
    key.writeSocket(buff, length);
}

void FoxTcpServerDemoHandler::handleDisconnect(FoxTcpSocketKey& key)
{
    // ������һ���ͻ�socket
    string message = STLStringUtils::snprintf("handleDisconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        key.getSocket()
    );
    logger->info(message);
}
