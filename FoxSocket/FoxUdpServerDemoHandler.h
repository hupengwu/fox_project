#pragma once

#include <ILogger.h>

#include "FoxSocketHandler.h"

class FoxUdpServerDemoHandler : public FoxSocketHandler
{
public:
    FoxUdpServerDemoHandler();
    ~FoxUdpServerDemoHandler();

public:
    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����NioServerSocket/NioClientSocket
     *
     * @param key SelectionKey
     */
    virtual void handleReadFrom(FoxSocketKey& serverKey, const void* buff, int buffLen, sockaddr_in& addr_client, int& addrLen);

private:
    /**
     * ��־
     */
    static ILogger* logger;

};