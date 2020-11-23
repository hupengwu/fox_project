#pragma once

#include <STLRunnable.h>
#include <ILogger.h>
#include <netinet/in.h>

#include "FoxTcpServerMapper.h"
#include "FoxTcpServerHandler.h"

class FoxTcpServerRecver : public STLRunnable
{
public:
	void run();

public:
	FoxTcpServerRecver(FoxTcpSocketKey& socketKey, FoxTcpServerHandler* handler);
	virtual ~FoxTcpServerRecver();

private:
	FoxTcpSocketKey			socketKey;
	FoxTcpServerHandler*	socketL1Handler;

private:
	static ILogger*			logger;
};


