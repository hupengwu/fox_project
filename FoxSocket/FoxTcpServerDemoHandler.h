#pragma once

#include <ILogger.h>

#include "FoxSocketHandler.h"

class FoxTcpServerDemoHandler : public FoxSocketHandler
{
public:
	FoxTcpServerDemoHandler();
	~FoxTcpServerDemoHandler();

public:
    /**
     * 处理Accept消息：当一个客户端接入服务端的时候，会捕获到这个动作
     * 对象：NioServerSocket
     *
     * @param key SelectionKey
     */
    virtual void handleConnect(FoxSocketKey& key);

    /**
     * 处理Read消息：当接收到客户端发送过来的数据时，会捕获到这个动作
     * 对象：NioServerSocket/NioClientSocket
     *
     * @param key SelectionKey
     */
    virtual void handleRead(FoxSocketKey& key, const void* buff, int length);

    /**
     * 处理NoRead消息：SELECT超过一定的时间间隔都没有收到该数据，会捕获到这个动作
     * 对象：
     *
     * @param key FoxStty
     */
    virtual void handleNoRead(FoxSocketKey& key);

     /**
      * 处理断开连接消息：当客户端跟服务器的连接断开时候，会捕获到这个动作
      * 对象：NioClientSocket
      *
      * @param key SelectionKey
      */
    virtual void handleDisconnect(FoxSocketKey& key);

private:
    /**
     * 日志
     */
    static ILogger* logger;

};

