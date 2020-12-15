#pragma once

#include <STLAsyncTask.h>
#include <ILogger.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"

#include "FoxSocket.h"

/**
 * �����Socket:���߳���Ӧ��socket
 * �ṹ��һ�������̣߳�������ݶ��̣߳�һ������д�߳�
 *
 * @author h00442047
 * @since 2019��12��2��
 */
class FoxTcpServerSocket : public FoxSocket
{
public:
    FoxTcpServerSocket();
    virtual ~FoxTcpServerSocket();

public:
    /*
    * ����/�ر�socket
    */
    bool create(int nSocketPort);
    void close();

    /*
    * ����accept/recv��ʱ
    */
    bool setRevTimeOut(timeval& timeout);

    /*
    * �����̵߳�����
    */
    void setThreads(int nThreads);
    int  getThreads();

protected:  
    /*
    * ������Ҫʵ�ֵĽ������ݴ�����
    */
    virtual void                recvFunc(STLThreadObject* socket);

    /*
    * �߳���
    */
    int                         nThreads;

    /*
    * ������ͻ��˵��̳߳�
    */
    STLAsyncTask                clientThread;
};

