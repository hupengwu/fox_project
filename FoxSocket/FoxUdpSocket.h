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
    * ����socket
    */
    bool create();

    /*
    * �󶨶˿�
    */
    bool bind(int localPort);

    /*
    * ��������
    */
    int sendTo(const char* buff, int buffLen, sockaddr_in& remoteAddr, int remoteAddrLen);
    int sendTo(const char* buff, int buffLen, const char* remoteIP, int remotePort);

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

