#pragma once

#include <mutex>
#include <thread>

#include <ILogger.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"


class FoxSocket
{
public:
    FoxSocket();
    virtual ~FoxSocket();

public:
    /*
    * 创建socket
    */
    virtual bool create(int nSocketPort);

    /*
    * 关闭socket
    */
    virtual void close();

    /*
    * 自定义socketHandler，它会被自动释放
    */
    bool                       bindSocketHandler(FoxSocketHandler* socketHandler);

protected:// 线程结束状况
    void				       setFinished(bool finished);
    bool				       getFinished();
    void				       setExit(bool isExit);
    bool				       getExit();

    bool				       isExit;    // 请求退出标识
    bool				       bFinished; // 是否已经退出线程

protected:
    /**
     * 日志
     */
    static ILogger*            logger;

    /**
     * 互斥
     */
    mutex				        lock;

    /**
     * 服务端socket的地址信息
     */
    FoxSocketKey                socketKey;

    /**
     * 外部接口
     */
    FoxSocketHandler*           socketHandler;

    /*
    * 负责监听客户端接入的线程函数
    */
    static  void			    recvThreadFunc(FoxSocket* socket);

    /*
    * 子类需要实现的接受数据处理函数
    */
    virtual void                recvFunc(FoxSocket* socket) {};

    /*
    * 负责监听客户端接入的线程
    */
    thread*                     recvThread;

};

