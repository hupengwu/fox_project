#pragma once

#include <ILogger.h>

#include "FoxSocketHandler.h"

class FoxUdpServerDemoHandler : public FoxSocketHandler
{
public:
    FoxUdpServerDemoHandler();
    ~FoxUdpServerDemoHandler();

public:
    /**
     * 处理Read消息：当接收到客户端发送过来的数据时，会捕获到这个动作
     * 对象：NioServerSocket/NioClientSocket
     *
     * @param key SelectionKey
     */
    virtual void handleReadFrom(FoxSocketKey& serverKey, const void* buff, int buffLen, sockaddr_in& addr_client, int& addrLen);

private:
    /**
     * 日志
     */
    static ILogger* logger;

};