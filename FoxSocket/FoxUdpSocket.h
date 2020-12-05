#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxSocketKey.h"
#include "FoxSocketHandler.h"
#include "FoxSocket.h"

/*
* UDPSocket：这是内置异步读数据线程的UDP的服务端/客户端封装对象，简化UDP的操作。
*/
class FoxOldUdpSocket : public FoxSocket
{
public:
    FoxOldUdpSocket();
    virtual ~FoxOldUdpSocket();

public:
    /*
    * 创建socket
    */
    bool create();

    /*
    * 绑定端口
    */
    bool bind(int localPort);

    /*
    * 发送数据
    */
    int sendTo(const char* buff, int buffLen, sockaddr_in& remoteAddr, int remoteAddrLen);
    int sendTo(const char* buff, int buffLen, const char* remoteIP, int remotePort);

    /*
    * 关闭socket
    */
    void close();

protected:
    /*
    * 子类需要实现的接受数据处理函数
    */
    virtual void               recvFunc(STLThreadObject* socket);

    /*
    * 接收缓存
    */
    char                       recvBuff[16 * 1024];

};

