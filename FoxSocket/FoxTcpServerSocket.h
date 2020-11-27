#pragma once

#include <STLAsyncTask.h>
#include <ILogger.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"

/**
 * �����Socket:���߳���Ӧ��socket
 * �ṹ��һ�������̣߳�������ݶ��̣߳�һ������д�߳�
 *
 * @author h00442047
 * @since 2019��12��2��
 */
class FoxTcpServerSocket
{
public:
    FoxTcpServerSocket();
    virtual ~FoxTcpServerSocket();

public:
    bool start(int nSocketPort);
    void close();

    void setThreads(int nThreads);
    int  getThreads();

public:
    int                 getServerSocket();
    sockaddr_in         getServerAddr();

    /*
    * �Զ���socketHandler�����ᱻ�Զ��ͷ�
    */
    bool                bindSocketHandler(FoxSocketHandler* socketHandler);

private:// �߳̽���״��
    void				setFinished(bool finished);
    bool				getFinished();
    void				setExit(bool isExit);
    bool				getExit();

    bool				isExit;    // �����˳���ʶ
    bool				bFinished; // �Ƿ��Ѿ��˳��߳�

private:  
    /**
     * ��־
     */
    static ILogger*             logger;

    /**
     * ���� 
     */
    mutex				        lock;

    /**
     * �����socket�ĵ�ַ��Ϣ
     */
    FoxSocketKey             socketKey;

    /*
    * �߳���
    */
    int                         nThreads;
private:
    /**
     * �ⲿ�ӿ�
     */
    FoxSocketHandler*        socketHandler;

    /*
    * ��������ͻ��˽�����̺߳���
    */
    static void			        recvThreadFunc(FoxTcpServerSocket& socket);
    /*
    * ��������ͻ��˽�����߳�
    */
    thread*                     recvThread;

    /*
    * ������ͻ��˵��̳߳�
    */
    STLAsyncTask                clientThread;
};

