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
    this->isExit = false;
    this->bFinished = true;
    this->socketHandler = nullptr;
    this->recvThread = nullptr;
    this->socketKey.setSocket(-1);

    this->socketHandler = new FoxSocketHandler();
}

FoxSocket::~FoxSocket()
{
    delete this->socketHandler;
    this->socketHandler = nullptr;
}

bool FoxSocket::create(int nSocketPort)
{
	return false;
}

void FoxSocket::close()
{
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

void FoxSocket::setFinished(bool finished)
{
    lock_guard<mutex> guard(this->lock);
    this->bFinished = finished;
}

bool FoxSocket::getFinished()
{
    lock_guard<mutex> guard(this->lock);
    return this->bFinished;
}

void FoxSocket::setExit(bool isExit)
{
    lock_guard<mutex> guard(this->lock);
    this->isExit = isExit;
}

bool FoxSocket::getExit()
{
    lock_guard<mutex> guard(this->lock);
    return this->isExit;
}

void FoxSocket::recvThreadFunc(FoxSocket* socket)
{
    while (true)
    {
        // 检查：退出线程标记
        if (socket->getExit())
        {
            break;
        }
        
        socket->recvFunc(socket);
    }

    // 退出线程
    FoxSocketKey socketKey = socket->socketKey;
    logger->info("finish recvThreadFunc from socket, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(socketKey.getSocketAddr().sin_addr),
        socketKey.getSocketAddr().sin_port,
        socketKey.getSocket());

    socket->setFinished(true);
}
