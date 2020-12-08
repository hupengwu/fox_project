#include "FoxUdpServerDemoHandler.h"

#include <FoxLoggerFactory.h>
#include <STLStringUtils.h>
#include <arpa/inet.h>

ILogger* FoxUdpServerDemoHandler::logger = FoxLoggerFactory::getLogger();

FoxUdpServerDemoHandler::FoxUdpServerDemoHandler()
{
}

FoxUdpServerDemoHandler::~FoxUdpServerDemoHandler()
{
}

void FoxUdpServerDemoHandler::handleReadFrom(FoxSocketKey& serverKey, const void* buff, int buffLen, sockaddr_in& addr_client, int& addrLen)
{
    static int i = 0;
    if (i++ < 100000)
    {
        // 接收到的数据
        logger->info("handleReadFrom from client, address : %s, port : %d ,Socket Num : % d,message =  %s; %d",
            inet_ntoa(addr_client.sin_addr),
            addr_client.sin_port,
            serverKey.getSocket(),
            buff,
            i);


        // 将数据原样返回
        serverKey.sendTo(buff, buffLen,  addr_client, addrLen);
    }
}

