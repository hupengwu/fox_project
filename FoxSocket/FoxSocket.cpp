#include "FoxSocket.h"

#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <arpa/inet.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <pthread.h>
#include <sys/errno.h>    // for errno
#include <unistd.h>

#include <STLStringUtils.h>
#include <FoxLoggerFactory.h>

#include "FoxSocket.h"
#include "FoxTcpServerRecver.h"
#include "FoxSocketKey.h"

ILogger* FoxSocket::logger = FoxLoggerFactory::getLogger();


FoxSocket::FoxSocket()
{    
    this->socketKey.setSocket(-1);

    this->socketHandler = nullptr;
    this->socketHandler = new FoxSocketHandler();
}

FoxSocket::~FoxSocket()
{
    delete this->socketHandler;
    this->socketHandler = nullptr;
}

bool FoxSocket::bindSocketHandler(FoxSocketHandler* socketHandler)
{
    lock_guard<mutex> guard(this->lock);

    if (socketHandler == nullptr)
    {
        return false;
    }

    delete this->socketHandler;
    this->socketHandler = socketHandler;

    return true;
}

FoxSocketKey FoxSocket::getSocketKey()
{
    lock_guard<mutex> guard(this->lock);
    return this->socketKey;
}
