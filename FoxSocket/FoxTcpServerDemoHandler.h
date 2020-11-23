#pragma once

#include <ILogger.h>

#include "FoxTcpServerHandler.h"

class FoxTcpServerDemoHandler : public FoxTcpServerHandler
{
public:
	FoxTcpServerDemoHandler();
	~FoxTcpServerDemoHandler();

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
    virtual void handleRead(FoxTcpSocketKey& key, const char* buff, int length);

     /**
      * ����Ͽ�������Ϣ�����ͻ��˸������������ӶϿ�ʱ�򣬻Ჶ���������
      * ����NioClientSocket
      *
      * @param key SelectionKey
      */
    virtual void handleDisconnect(FoxTcpSocketKey& key);

private:
    /**
     * ��־
     */
    static ILogger* logger;

};

