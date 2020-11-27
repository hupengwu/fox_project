#pragma once

#include <STLRunnable.h>
#include <ILogger.h>
#include <netinet/in.h>

#include "FoxTcpServerMapper.h"
#include "FoxSocketHandler.h"

/*
* ���߳̽����ߣ�socket�Ľ��հ�������/�Ͽ�/������������
*/
class FoxTcpServerRecver : public STLRunnable
{
public:
	void run();

public:
	FoxTcpServerRecver(FoxSocketKey& socketKey, FoxSocketHandler* socketHandler);
	virtual ~FoxTcpServerRecver();

private:
	FoxSocketKey			socketKey;
	FoxSocketHandler*		socketHandler;

private:
	static ILogger*			logger;
};


