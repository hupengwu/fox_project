#pragma once

#include <mutex>

#include "FoxTcpSocketKey.h"

using namespace std;

/**
 * 捕获数据的数据接口
 *
 * @author h00442047
 * @since 2019年12月10日
 */
class FoxTcpSocketHandler
{
public:
    FoxTcpSocketHandler();
    virtual ~FoxTcpSocketHandler();

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
     * @param key SelectionKey
     */
    virtual void handleConnect(FoxTcpSocketKey& key);

    /**
     * 处理Read消息：当接收到客户端发送过来的数据时，会捕获到这个动作
     * 对象：FoxTcpServerSocket
     *
     * @param key SelectionKey
     */
    virtual void handleRead(FoxTcpSocketKey& key,const char* buff,int length);

    /**
     * 处理Write消息：当可以给客户端发送数据的时候，会捕获到这个动作
     * 对象：FoxTcpServerSocket
     *
     * @param key SelectionKey
     */
    //virtual void handleWrite(FoxTcpSocketKey& key) = 0;

    /**
     * 处理断开连接消息：当客户端跟服务器的连接断开时候，会捕获到这个动作
     * 对象：FoxTcpServerSocket
     *
     * @param key SelectionKey
     */
    virtual void handleDisconnect(FoxTcpSocketKey& key);

private:
    mutex		 lock;
    bool		 isExit;    // 请求退出标识
};

