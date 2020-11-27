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
    * ����socket
    */
    bool create(int serverPort);

    /*
    * ��������
    */
    int sendTo(const char* buff, int buffLen, sockaddr_in& addr_client, int addrLen);

    /*
    * �ر�socket
    */
    void close();

protected:
    /*
    * ������Ҫʵ�ֵĽ������ݴ�����
    */
    virtual void               recvFunc(FoxSocket* socket);

    /*
    * ���ջ���
    */
    char                       recvBuff[16 * 1024];

};

