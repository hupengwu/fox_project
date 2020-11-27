#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxSocketKey.h"
#include "FoxSocketHandler.h"
#include "FoxSocket.h"

class FoxUdpServerSocket : public FoxSocket
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
    int sendTo(const char* buff, int buffLen, sockaddr_in& addr_client, int addrLen);

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

