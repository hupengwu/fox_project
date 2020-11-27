#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxSocketKey.h"
#include "FoxSocketHandler.h"


class FoxUdpServerSocket
{
public:
    FoxUdpServerSocket();
    virtual ~FoxUdpServerSocket();

public:
    /*
    * 创建socket
    */
    bool create(int serverPort);

    /*
    * 发送数据
    */
    int send(const char* buff, int length);

    /*
    * 关闭socket
    */
    void close();

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
    FoxSocketKey                socketKey;

private:
    /**
     * 外部接口
     */
    FoxSocketHandler*           socketHandler;

    /*
    * 负责监听客户端接入的线程函数
    */
    static void			        recvThreadFunc(FoxUdpServerSocket& socket);
    /*
    * 负责监听客户端接入的线程
    */
    thread* recvThread;
};

