#pragma once

#include <mutex>
#include <thread>
#include <ILogger.h>

#include "FoxTcpSocketKey.h"
#include "FoxTcpSocketHandler.h"

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

    /*
    * �ر�socket
    */
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
    thread*                     recvThread;
};

