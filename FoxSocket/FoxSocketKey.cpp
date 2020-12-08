#include "FoxSocketKey.h"
#include <unistd.h>

FoxSocketKey::FoxSocketKey()
{
    this->invalid = false;
	this->socket = -1;
}

FoxSocketKey::FoxSocketKey(const FoxSocketKey& src)
{
	this->invalid = src.invalid;
	this->socket = src.socket;
	this->sockaddr = src.sockaddr;
}

FoxSocketKey::~FoxSocketKey()
{
}

const FoxSocketKey& FoxSocketKey::operator=(const FoxSocketKey& src)
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

sockaddr_in FoxSocketKey::getSocketAddr()
{
    return this->sockaddr;
}

void FoxSocketKey::setSocketAddr(sockaddr_in sockaddr)
{
    this->sockaddr = sockaddr;
}

int FoxSocketKey::getSocket()
{
    return this->socket;
}

void FoxSocketKey::setSocket(int socket)
{
    this->socket = socket;
}

void FoxSocketKey::setInvalid(bool invalid)
{
    this->invalid = invalid;
}

bool FoxSocketKey::getInvalid()
{
    return this->invalid;
}

int FoxSocketKey::send(const void* buff, int length)
{    
    return ::send(this->socket, buff, length, 0);
}

int FoxSocketKey::sendTo(const void* buff, int buffLen, sockaddr_in& addr_client, int addrLen)
{
	return ::sendto(this->socket, buff, buffLen, 0, (struct sockaddr*)&addr_client, addrLen);
}
