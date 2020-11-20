#pragma once

#include <string>
//#include <ILogger.h>

class FoxTcpServerSocket
{
private:
	//static ILogger* logger;

public:
	bool create(const char* serverIp, unsigned int socketPort);
};

