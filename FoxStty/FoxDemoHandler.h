#pragma once

#include "FoxSttyHandler.h"

class FoxDemoHandler : public FoxSttyHandler
{
public:
	FoxDemoHandler();
	virtual ~FoxDemoHandler();

public:
	virtual void handleRead(int fd, const unsigned char* buff, int length);
};

