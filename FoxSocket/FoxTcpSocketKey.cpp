#include "FoxTcpSocketKey.h"
#include <unistd.h>

FoxTcpSocketKey::FoxTcpSocketKey()
{
    this->invalid = false;
	this->socket = -1;
}

FoxTcpSocketKey::FoxTcpSocketKey(const FoxTcpSocketKey& src)
{
	this->invalid = src.invalid;
	this->socket = src.socket;
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

	this->invalid = src.invalid;
	this->socket = src.socket;
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
    return this->socket;
}

void FoxTcpSocketKey::setSocket(int socket)
{
    this->socket = socket;
}

void FoxTcpSocketKey::setInvalid(bool invalid)
{
    this->invalid = invalid;
}

bool FoxTcpSocketKey::getInvalid()
{
    return this->invalid;
}

int FoxTcpSocketKey::writeSocket(const char* buff, int length)
{    
    return ::send(this->socket, buff, length, 0);
}
