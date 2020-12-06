#pragma once

#include "FoxSttyHandler.h"
#include "FoxSttyBuffer.h"

class FoxDemoHandler : public FoxSttyHandler
{
public:
	FoxDemoHandler();
	virtual ~FoxDemoHandler();

public:
	virtual void handleRead(int fd, const unsigned char* buff, int length);

    virtual void handleNoRead(int fd);

private:
	FoxSttyBuffer recver;
};

