#pragma once

#include <mutex>

#include "FoxTcpSocketKey.h"

using namespace std;

/**
 * �������ݵ����ݽӿ�
 *
 * @author h00442047
 * @since 2019��12��10��
 */
class FoxTcpServerHandler
{
public:
    FoxTcpServerHandler();
    virtual ~FoxTcpServerHandler();

public:
    /**
     * ����״̬
     * ����NioServerSocket/NioClientSocket
     *
     * @return �Ƿ����еı��
     */
    void				setExit(bool isExit);
    bool				getExit();

public:
    /**
     * ����Accept��Ϣ����һ���ͻ��˽������˵�ʱ�򣬻Ჶ���������
     * ����NioServerSocket
     *
     * @param key SelectionKey
     */
    virtual void handleConnect(FoxTcpSocketKey& key);

    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����NioServerSocket/NioClientSocket
     *
     * @param key SelectionKey
     */
    virtual void handleRead(FoxTcpSocketKey& key,const char* buff,int length);

    /**
     * ����Write��Ϣ�������Ը��ͻ��˷������ݵ�ʱ�򣬻Ჶ���������
     * ����NioServerSocket/NioClientSocket
     *
     * @param key SelectionKey
     */
    //virtual void handleWrite(FoxTcpSocketKey& key) = 0;

    /**
     * ����Ͽ�������Ϣ�����ͻ��˸������������ӶϿ�ʱ�򣬻Ჶ���������
     * ����NioClientSocket
     *
     * @param key SelectionKey
     */
    virtual void handleDisconnect(FoxTcpSocketKey& key);

private:
    mutex		 lock;
    bool		 isExit;    // �����˳���ʶ
};

