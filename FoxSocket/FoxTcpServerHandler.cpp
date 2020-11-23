#include "FoxTcpServerHandler.h"

FoxTcpServerHandler::FoxTcpServerHandler()
{
	this->isExit = false;
}

FoxTcpServerHandler::~FoxTcpServerHandler()
{
}

bool FoxTcpServerHandler::getExit()
{
	lock_guard<mutex> guard(this->lock);
	return this->isExit;
}

void FoxTcpServerHandler::setExit(bool isExit)
{
	lock_guard<mutex> guard(this->lock);
	this->isExit = isExit;
}

void FoxTcpServerHandler::handleConnect(FoxTcpSocketKey& key)
{
}

void FoxTcpServerHandler::handleRead(FoxTcpSocketKey& key, const char* buff, int length)
{
}

void FoxTcpServerHandler::handleDisconnect(FoxTcpSocketKey& key)
{
}
