#pragma once

#include <mutex>
#include <thread>

#include <ILogger.h>
#include <STLThreadObject.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"

/*
* socket多线程封装：简化socket的操作，通过异步线程提供一个通用的消息响应机制
* 
* https://www.cnblogs.com/wangfengju/p/6172575.html
*/
class FoxSocket : public STLThreadObject
{
public:
    FoxSocket();
    virtual ~FoxSocket();

public:
    /*
    * 自定义socketHandler，它会被自动释放
    */
    bool                       bindHandler(FoxSocketHandler* handler);

    /*
    * 获取SocketKey信息
    */
    FoxSocketKey               getSocketKey();

protected:
    /**
     * 日志
     */
    static ILogger*            logger;

    /**
     * 服务端socket的地址信息
     */
    FoxSocketKey               socketKey;

    /**
     * 外部接口
     */
    FoxSocketHandler*          socketHandler;  

    /*
    * 互斥
    */
    mutex				       lock;
};

