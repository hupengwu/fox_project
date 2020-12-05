#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxSocketKey.h"
#include "FoxSocketHandler.h"
#include "FoxSocket.h"

/**
 * �ͻ���socket����̨�߳���Ӧ��socket
 * �ṹ��һ����̨���ݶ��߳�
 * socketChannel����
 * 1.����ͨ��connect�ɹ�����֪����������ЩsocketChannel;
 * 2.����ͨ���Զ����SocketCloseHandler��������ЩsocketChannel�ر�;
 * 3.����ͨ��isConnected����ָ����ǰsocketChannel��;�Ƿ�ʧЧ
 * 4.����ͨ��SocketConnectHandler��������ЩsocketChannel��������;
 *
 * @author h00442047
 * @since 2019��12��25��
 */
class FoxTcpClientSocket : public FoxSocket
{
public:
    FoxTcpClientSocket();
    virtual ~FoxTcpClientSocket();

public:
    /*
    * �ر�socket
    */
    void close();

    /*
    * ���ӷ�����
    */
	bool connect(const char* remoteIP, int remotePort);

    /*
    * ��������
    */
    int send(const char* buff, int length);    

protected:
    /*
    * ������Ҫʵ�ֵĽ������ݴ�����
    */
    virtual void               recvFunc(STLThreadObject* socket);

    char                       recvBuff[16 * 1024];
};

