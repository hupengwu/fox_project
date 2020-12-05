#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxSocketKey.h"
#include "FoxSocketHandler.h"
#include "FoxSocket.h"

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
class FoxTcpClientSocket : public FoxSocket
{
public:
    FoxTcpClientSocket();
    virtual ~FoxTcpClientSocket();

public:
    /*
    * 关闭socket
    */
    void close();

    /*
    * 连接服务器
    */
	bool connect(const char* remoteIP, int remotePort);

    /*
    * 发送数据
    */
    int send(const char* buff, int length);    

protected:
    /*
    * 子类需要实现的接受数据处理函数
    */
    virtual void               recvFunc(STLThreadObject* socket);

    char                       recvBuff[16 * 1024];
};

