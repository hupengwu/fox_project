#pragma once

#include <STLAsyncTask.h>
#include <ILogger.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"

#include "FoxSocket.h"

/**
 * 服务端Socket:多线程响应的socket
 * 结构：一个监听线程，多个数据读线程，一个数据写线程
 *
 * @author h00442047
 * @since 2019年12月2日
 */
class FoxTcpServerSocket : public FoxSocket
{
public:
    FoxTcpServerSocket();
    virtual ~FoxTcpServerSocket();

public:
    /*
    * 创建/关闭socket
    */
    bool create(int nSocketPort);
    void close();

    /*
    * 设置accept/recv超时
    */
    bool setRevTimeOut(timeval& timeout);

    /*
    * 接收线程的数量
    */
    void setThreads(int nThreads);
    int  getThreads();

protected:  
    /*
    * 子类需要实现的接受数据处理函数
    */
    virtual void                recvFunc(STLThreadObject* socket);

    /*
    * 线程数
    */
    int                         nThreads;

    /*
    * 负责处理客户端的线程池
    */
    STLAsyncTask                clientThread;
};

