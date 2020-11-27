#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxSocketKey.h"
#include "FoxSocketHandler.h"
#include "FoxSocket.h"

class FoxUdpSocket : public FoxSocket
{
public:
    FoxUdpSocket();
    virtual ~FoxUdpSocket();

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
    virtual void               recvFunc(FoxSocket* socket);

    /*
    * 接收缓存
    */
    char                       recvBuff[16 * 1024];

};

