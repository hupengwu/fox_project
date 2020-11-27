#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxSocketKey.h"
#include "FoxSocketHandler.h"


class FoxUdpServerSocket
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
    int send(const char* buff, int length);

    /*
    * �ر�socket
    */
    void close();

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
    FoxSocketKey                socketKey;

private:
    /**
     * �ⲿ�ӿ�
     */
    FoxSocketHandler*           socketHandler;

    /*
    * ��������ͻ��˽�����̺߳���
    */
    static void			        recvThreadFunc(FoxUdpServerSocket& socket);
    /*
    * ��������ͻ��˽�����߳�
    */
    thread* recvThread;
};

