#pragma once

#include <mutex>

#include "FoxSocketKey.h"

using namespace std;

/**
 * 捕获数据的数据接口
 *
 * @author h00442047
 * @since 2019年12月10日
 */
class FoxSocketHandler
{
public:
    FoxSocketHandler();
    virtual ~FoxSocketHandler();

public:
    /**
     * 运行状态
     * 对象：FoxTcpServerSocket
     *
     * @return 是否运行的标记
     */
    void				setExit(bool isExit);
    bool				getExit();

public:
    /**
     * 处理Accept消息：当一个客户端接入服务端的时候，会捕获到这个动作
     * 对象：FoxTcpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleConnect(FoxSocketKey& key);

    /**
     * 处理Read消息：当接收到客户端发送过来的数据时，会捕获到这个动作
     * 对象：FoxTcpServerSocket/FoxUdpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleRead(FoxSocketKey& key,const char* buff,int length);

    /**
     * 处理Read消息：当接收到客户端发送过来的数据时，会捕获到这个动作
     * 对象：FoxUdpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleReadFrom(FoxSocketKey& serverKey, const char* buff, int buffLen, sockaddr_in& addr_client, int& addrLen);

    /**
     * 处理断开连接消息：当客户端跟服务器的连接断开时候，会捕获到这个动作
     * 对象：FoxTcpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleDisconnect(FoxSocketKey& key);

    /**
     * 处理关闭Socket消息：当服务端关闭的时候，会捕获到这个动作
     * 对象：FoxTcpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleClosed(FoxSocketKey& key);

private:
    mutex		 lock;
    bool		 isExit;    // 请求退出标识
};

