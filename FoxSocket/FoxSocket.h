#pragma once

#include <mutex>
#include <thread>

#include <ILogger.h>
#include <STLThreadObject.h>

#include "FoxSocketHandler.h"
#include "FoxSocketKey.h"


class FoxSocket : public STLThreadObject
{
public:
    FoxSocket();
    virtual ~FoxSocket();

public:
    /*
    * �Զ���socketHandler�����ᱻ�Զ��ͷ�
    */
    bool                       bindHandler(FoxSocketHandler* handler);

    /*
    * ��ȡSocketKey��Ϣ
    */
    FoxSocketKey               getSocketKey();

protected:
    /**
     * ��־
     */
    static ILogger*            logger;

    /**
     * �����socket�ĵ�ַ��Ϣ
     */
    FoxSocketKey               socketKey;

    /**
     * �ⲿ�ӿ�
     */
    FoxSocketHandler*          socketHandler;  

    /*
    * ����
    */
    mutex				       lock;
};

