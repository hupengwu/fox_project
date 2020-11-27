#pragma once

#include <mutex>
#include <thread>

#include <ILogger.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"


class FoxSocket
{
public:
    FoxSocket();
    virtual ~FoxSocket();

public:
    /*
    * ����socket
    */
    virtual bool create(int nSocketPort);

    /*
    * �ر�socket
    */
    virtual void close();

    /*
    * �Զ���socketHandler�����ᱻ�Զ��ͷ�
    */
    bool                       bindSocketHandler(FoxSocketHandler* socketHandler);

protected:// �߳̽���״��
    void				       setFinished(bool finished);
    bool				       getFinished();
    void				       setExit(bool isExit);
    bool				       getExit();

    bool				       isExit;    // �����˳���ʶ
    bool				       bFinished; // �Ƿ��Ѿ��˳��߳�

protected:
    /**
     * ��־
     */
    static ILogger*            logger;

    /**
     * ����
     */
    mutex				        lock;

    /**
     * �����socket�ĵ�ַ��Ϣ
     */
    FoxSocketKey                socketKey;

    /**
     * �ⲿ�ӿ�
     */
    FoxSocketHandler*           socketHandler;

    /*
    * ��������ͻ��˽�����̺߳���
    */
    static  void			    recvThreadFunc(FoxSocket* socket);

    /*
    * ������Ҫʵ�ֵĽ������ݴ�����
    */
    virtual void                recvFunc(FoxSocket* socket) {};

    /*
    * ��������ͻ��˽�����߳�
    */
    thread*                     recvThread;

};

