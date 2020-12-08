#include "FoxSocketHandler.h"

FoxSocketHandler::FoxSocketHandler()
{
	this->isExit = false;
}

FoxSocketHandler::~FoxSocketHandler()
{
}

bool FoxSocketHandler::getExit()
{
	lock_guard<mutex> guard(this->lock);
	return this->isExit;
}

void FoxSocketHandler::setExit(bool isExit)
{
	lock_guard<mutex> guard(this->lock);
	this->isExit = isExit;
}

void FoxSocketHandler::handleConnect(FoxSocketKey& key)
{
}

void FoxSocketHandler::handleRead(FoxSocketKey& key, const void* buff, int length)
{
}

void FoxSocketHandler::handleReadFrom(FoxSocketKey& serverKey, const void* buff, int length, sockaddr_in& addr_client, int& addrLen)
{
}

void FoxSocketHandler::handleDisconnect(FoxSocketKey& key)
{
}

void FoxSocketHandler::handleClosed(FoxSocketKey& key)
{
}
