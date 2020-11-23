#pragma once

#include <STLAsyncTask.h>
#include <ILogger.h>

#include "FoxTcpServerHandler.h"

class FoxTcpServerSocket
{
public:
    FoxTcpServerSocket();
    virtual ~FoxTcpServerSocket();

public:
    bool start(int nSocketPort);
    void close();

public:
    int                 getServerSocket();
    sockaddr_in         getServerAddr();

    /*
    * 自定义socketHandler，它会被自动释放
    */
    bool bindSocketHandler(FoxTcpServerHandler* socketHandler);

private:// 线程结束状况
    void				setFinished(bool finished);
    bool				getFinished();
    void				setExit(bool isExit);
    bool				getExit();

    bool				isExit;    // 请求退出标识
    bool				bFinished; // 是否已经退出线程

private:
    /**
     * 外部接口
     */
    FoxTcpServerHandler* socketHandler;

private:  
    /**
     * 日志
     */
    static ILogger*     logger;

    /**
     * 互斥 
     */
    mutex				lock;

    /**
     * 查询消息间隔
     */
    int                 timeOut = 1 * 1000;

    /**
     * 服务端socket的地址信息
     */
    sockaddr_in         serverAddr;

    /**
     * 服务端socket句柄
     */
    int                 serverSocket;
private:
    /*
    * 负责监听客户端接入的线程函数
    */
    static void			        listenThreadFunc(FoxTcpServerSocket& socket);
    /*
    * 负责监听客户端接入的线程
    */
    thread*                     listenThread;

    /*
    * 负责处理客户端的线程池
    */
    STLAsyncTask                clientThread;
};

