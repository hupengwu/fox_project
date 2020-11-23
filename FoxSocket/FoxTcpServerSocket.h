#pragma once

#include <STLAsyncTask.h>
#include <ILogger.h>

#include "FoxTcpServerHandler.h"

class FoxTcpServerSocket
{
public:
    FoxTcpServerSocket();
    virtual ~FoxTcpServerSocket();

public:
    bool start(int nSocketPort);
    void close();

public:
    int                 getServerSocket();
    sockaddr_in         getServerAddr();

    /*
    * �Զ���socketHandler�����ᱻ�Զ��ͷ�
    */
    bool bindSocketHandler(FoxTcpServerHandler* socketHandler);

private:// �߳̽���״��
    void				setFinished(bool finished);
    bool				getFinished();
    void				setExit(bool isExit);
    bool				getExit();

    bool				isExit;    // �����˳���ʶ
    bool				bFinished; // �Ƿ��Ѿ��˳��߳�

private:
    /**
     * �ⲿ�ӿ�
     */
    FoxTcpServerHandler* socketHandler;

private:  
    /**
     * ��־
     */
    static ILogger*     logger;

    /**
     * ���� 
     */
    mutex				lock;

    /**
     * ��ѯ��Ϣ���
     */
    int                 timeOut = 1 * 1000;

    /**
     * �����socket�ĵ�ַ��Ϣ
     */
    sockaddr_in         serverAddr;

    /**
     * �����socket���
     */
    int                 serverSocket;
private:
    /*
    * ��������ͻ��˽�����̺߳���
    */
    static void			        listenThreadFunc(FoxTcpServerSocket& socket);
    /*
    * ��������ͻ��˽�����߳�
    */
    thread*                     listenThread;

    /*
    * ������ͻ��˵��̳߳�
    */
    STLAsyncTask                clientThread;
};

