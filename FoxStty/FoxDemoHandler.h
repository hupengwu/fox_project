#pragma once

#include "FoxSttyHandler.h"
#include <STLFIFOBuffers.h>

class FoxDemoHandler : public FoxSttyHandler
{
public:
	FoxDemoHandler();
	virtual ~FoxDemoHandler();

public:
	virtual void handleRead(const int fd, const char* name, const unsigned char* buff, int length);

    virtual void handleNoRead(const int fd, const char* name);

private:
	STLFIFOBuffers buffers;
};

