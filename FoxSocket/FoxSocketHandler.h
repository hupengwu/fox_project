#pragma once

#include <mutex>

#include "FoxSocketKey.h"

using namespace std;

/**
 * �������ݵ����ݽӿ�
 *
 * @author h00442047
 * @since 2019��12��10��
 */
class FoxSocketHandler
{
public:
    FoxSocketHandler();
    virtual ~FoxSocketHandler();

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
     * @param key FoxSocketKey
     */
    virtual void handleConnect(FoxSocketKey& key);

    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����FoxTcpServerSocket/FoxUdpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleRead(FoxSocketKey& key,const char* buff,int length);

    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����FoxUdpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleReadFrom(FoxSocketKey& serverKey, const char* buff, int buffLen, sockaddr_in& addr_client, int& addrLen);

    /**
     * ����Ͽ�������Ϣ�����ͻ��˸������������ӶϿ�ʱ�򣬻Ჶ���������
     * ����FoxTcpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleDisconnect(FoxSocketKey& key);

    /**
     * ����ر�Socket��Ϣ��������˹رյ�ʱ�򣬻Ჶ���������
     * ����FoxTcpServerSocket
     *
     * @param key FoxSocketKey
     */
    virtual void handleClosed(FoxSocketKey& key);

private:
    mutex		 lock;
    bool		 isExit;    // �����˳���ʶ
};

