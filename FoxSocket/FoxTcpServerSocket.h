#pragma once

#include <STLAsyncTask.h>
#include <ILogger.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"

/**
 * 服务端Socket:多线程响应的socket
 * 结构：一个监听线程，多个数据读线程，一个数据写线程
 *
 * @author h00442047
 * @since 2019年12月2日
 */
class FoxTcpServerSocket
{
public:
    FoxTcpServerSocket();
    virtual ~FoxTcpServerSocket();

public:
    bool start(int nSocketPort);
    void close();

    void setThreads(int nThreads);
    int  getThreads();

public:
    int                 getServerSocket();
    sockaddr_in         getServerAddr();

    /*
    * 自定义socketHandler，它会被自动释放
    */
    bool                bindSocketHandler(FoxSocketHandler* socketHandler);

private:// 线程结束状况
    void				setFinished(bool finished);
    bool				getFinished();
    void				setExit(bool isExit);
    bool				getExit();

    bool				isExit;    // 请求退出标识
    bool				bFinished; // 是否已经退出线程

private:  
    /**
     * 日志
     */
    static ILogger*             logger;

    /**
     * 互斥 
     */
    mutex				        lock;

    /**
     * 服务端socket的地址信息
     */
    FoxSocketKey             socketKey;

    /*
    * 线程数
    */
    int                         nThreads;
private:
    /**
     * 外部接口
     */
    FoxSocketHandler*        socketHandler;

    /*
    * 负责监听客户端接入的线程函数
    */
    static void			        recvThreadFunc(FoxTcpServerSocket& socket);
    /*
    * 负责监听客户端接入的线程
    */
    thread*                     recvThread;

    /*
    * 负责处理客户端的线程池
    */
    STLAsyncTask                clientThread;
};

