#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxTcpSocketKey.h"
#include "FoxTcpSocketHandler.h"

/**
 * 客户端socket：后台线程响应的socket
 * 结构：一个后台数据读线程
 * socketChannel管理：
 * 1.可以通过connect成功，来知道创建了哪些socketChannel;
 * 2.可以通过自定义的SocketCloseHandler，捕获哪些socketChannel关闭;
 * 3.可以通过isConnected，来指导当前socketChannel中途是否失效
 * 4.可以通过SocketConnectHandler，捕获哪些socketChannel连接上了;
 *
 * @author h00442047
 * @since 2019年12月25日
 */
class FoxTcpClientSocket
{
public:
    FoxTcpClientSocket();
    virtual ~FoxTcpClientSocket();

public:
    /*
    * 连接服务器
    */
	bool connect(const char* serverIP, int serverPort);

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
    bool                bindSocketHandler(FoxTcpSocketHandler* socketHandler);

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
    FoxTcpSocketKey             socketKey;

private:
    /**
     * 外部接口
     */
    FoxTcpSocketHandler*        socketHandler;

    /*
    * 负责监听客户端接入的线程函数
    */
    static void			        recvThreadFunc(FoxTcpClientSocket& socket);
    /*
    * 负责监听客户端接入的线程
    */
    thread*                     recvThread;
};

