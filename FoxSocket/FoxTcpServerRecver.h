#pragma once

#include <STLRunnable.h>
#include <ILogger.h>
#include <netinet/in.h>

#include "FoxTcpServerMapper.h"
#include "FoxTcpSocketHandler.h"

/*
* ���߳̽����ߣ�socket�Ľ��հ�������/�Ͽ�/������������
*/
class FoxTcpServerRecver : public STLRunnable
{
public:
	void run();

public:
	FoxTcpServerRecver(FoxTcpSocketKey& socketKey, FoxTcpSocketHandler* socketHandler);
	virtual ~FoxTcpServerRecver();

private:
	FoxTcpSocketKey			socketKey;
	FoxTcpSocketHandler*	socketHandler;

private:
	static ILogger*			logger;
};


