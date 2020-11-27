#pragma once

#include <ILogger.h>

#include "FoxSocketHandler.h"

class FoxTcpServerDemoHandler : public FoxSocketHandler
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
    virtual void handleConnect(FoxSocketKey& key);

    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����NioServerSocket/NioClientSocket
     *
     * @param key SelectionKey
     */
    virtual void handleRead(FoxSocketKey& key, const char* buff, int length);

     /**
      * ����Ͽ�������Ϣ�����ͻ��˸������������ӶϿ�ʱ�򣬻Ჶ���������
      * ����NioClientSocket
      *
      * @param key SelectionKey
      */
    virtual void handleDisconnect(FoxSocketKey& key);

private:
    /**
     * ��־
     */
    static ILogger* logger;

};

