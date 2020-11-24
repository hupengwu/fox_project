#pragma once

#include <STLRunnable.h>
#include <ILogger.h>
#include <netinet/in.h>

#include "FoxTcpServerMapper.h"
#include "FoxTcpSocketHandler.h"

/*
* 多线程接收者：socket的接收包括连接/断开/数据三个动作
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


