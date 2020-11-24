#include "FoxTcpSocketHandler.h"

FoxTcpSocketHandler::FoxTcpSocketHandler()
{
	this->isExit = false;
}

FoxTcpSocketHandler::~FoxTcpSocketHandler()
{
}

bool FoxTcpSocketHandler::getExit()
{
	lock_guard<mutex> guard(this->lock);
	return this->isExit;
}

void FoxTcpSocketHandler::setExit(bool isExit)
{
	lock_guard<mutex> guard(this->lock);
	this->isExit = isExit;
}

void FoxTcpSocketHandler::handleConnect(FoxTcpSocketKey& key)
{
}

void FoxTcpSocketHandler::handleRead(FoxTcpSocketKey& key, const char* buff, int length)
{
}

void FoxTcpSocketHandler::handleDisconnect(FoxTcpSocketKey& key)
{
}
