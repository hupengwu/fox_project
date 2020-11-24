#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxTcpSocketKey.h"
#include "FoxTcpSocketHandler.h"

class FoxTcpClientSocket
{
public:
    FoxTcpClientSocket();
    virtual ~FoxTcpClientSocket();

public:
    /*
    * ���ӷ�����
    */
	bool connect(const char* serverIP, int serverPort);

    /*
    * ��������
    */
    int send(const char* buff, int length);

    void close();

    /*
    * �Զ���socketHandler�����ᱻ�Զ��ͷ�
    */
    bool                bindSocketHandler(FoxTcpSocketHandler* socketHandler);

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
    FoxTcpSocketKey             socketKey;

private:
    /**
     * �ⲿ�ӿ�
     */
    FoxTcpSocketHandler*        socketHandler;

    /*
    * ��������ͻ��˽�����̺߳���
    */
    static void			        recvThreadFunc(FoxTcpClientSocket& socket);
    /*
    * ��������ͻ��˽�����߳�
    */
    thread*                     listenThread;
};

