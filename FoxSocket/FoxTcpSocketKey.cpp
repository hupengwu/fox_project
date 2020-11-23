#include "FoxTcpSocketKey.h"
#include <unistd.h>

FoxTcpSocketKey::FoxTcpSocketKey()
{
    this->closed = false;
	this->clientSocket = -1;
}

FoxTcpSocketKey::FoxTcpSocketKey(const FoxTcpSocketKey& src)
{
	this->closed = src.closed;
	this->clientSocket = src.clientSocket;
	this->sockaddr = src.sockaddr;
}

FoxTcpSocketKey::~FoxTcpSocketKey()
{
}

const FoxTcpSocketKey& FoxTcpSocketKey::operator=(const FoxTcpSocketKey& src)
{
	if (&src == this)
	{
		return *this;
	}

	this->closed = src.closed;
	this->clientSocket = src.clientSocket;
	this->sockaddr = src.sockaddr;

	return *this;
}

sockaddr_in FoxTcpSocketKey::getSocketAddr()
{
    return this->sockaddr;
}

void FoxTcpSocketKey::setSocketAddr(sockaddr_in sockaddr)
{
    this->sockaddr = sockaddr;
}

int FoxTcpSocketKey::getSocket()
{
    return this->clientSocket;
}

void FoxTcpSocketKey::setSocket(int clientSocket)
{
    this->clientSocket = clientSocket;
}

void FoxTcpSocketKey::setClosed()
{
    this->closed = true;
}

bool FoxTcpSocketKey::getClosed()
{
    return this->closed;
}

int FoxTcpSocketKey::writeSocket(const char* buff, int length)
{    
    return ::send(this->clientSocket, buff, length, 0);
}
