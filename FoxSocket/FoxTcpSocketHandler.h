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
class FoxTcpSocketHandler
{
public:
    FoxTcpSocketHandler();
    virtual ~FoxTcpSocketHandler();

public:
    /**
     * ����״̬
     * ����FoxTcpServerSocket
     *
     * @return �Ƿ����еı��
     */
    void				setExit(bool isExit);
    bool				getExit();

public:
    /**
     * ����Accept��Ϣ����һ���ͻ��˽������˵�ʱ�򣬻Ჶ���������
     * ����FoxTcpServerSocket
     *
     * @param key SelectionKey
     */
    virtual void handleConnect(FoxTcpSocketKey& key);

    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����FoxTcpServerSocket
     *
     * @param key SelectionKey
     */
    virtual void handleRead(FoxTcpSocketKey& key,const char* buff,int length);

    /**
     * ����Write��Ϣ�������Ը��ͻ��˷������ݵ�ʱ�򣬻Ჶ���������
     * ����FoxTcpServerSocket
     *
     * @param key SelectionKey
     */
    //virtual void handleWrite(FoxTcpSocketKey& key) = 0;

    /**
     * ����Ͽ�������Ϣ�����ͻ��˸������������ӶϿ�ʱ�򣬻Ჶ���������
     * ����FoxTcpServerSocket
     *
     * @param key SelectionKey
     */
    virtual void handleDisconnect(FoxTcpSocketKey& key);

private:
    mutex		 lock;
    bool		 isExit;    // �����˳���ʶ
};

