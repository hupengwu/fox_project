#include "FoxDemoHandler.h"
#include <string>

FoxDemoHandler::FoxDemoHandler()
{
}

FoxDemoHandler::~FoxDemoHandler()
{
}

void FoxDemoHandler::handleRead(int fd, const unsigned char* buff, int length)
{
	::printf("%s\n", buff);
}

void FoxDemoHandler::handleNoRead(int fd)
{
	::printf("no data\n");
}
