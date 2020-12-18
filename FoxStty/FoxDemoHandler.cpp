#include "FoxDemoHandler.h"
#include <string>

FoxDemoHandler::FoxDemoHandler()
{
}

FoxDemoHandler::~FoxDemoHandler()
{
}

void FoxDemoHandler::handleRead(const int fd, const char* name, const unsigned char* buff, int length)
{
	if (buff == nullptr || length <= 0)
	{
		return;
	}

	int size = 0;
	this->buffers.queryBuff(fd, size);
	if (size > 64 * 1024)
	{

	}

	this->buffers.appendBuff(fd, (char*)buff, length);

	::printf("%s\n", buff);
}

void FoxDemoHandler::handleNoRead(const int fd, const char* name)
{
	// 检查：是否为空数据
	int size = 0;
	if (!this->buffers.queryBuff(fd, size))
	{
		return;
	}

	STLByteArray data;
	this->buffers.removeBuff(fd, data);

	data.add(0);

	::printf("recv line:%s\n", data.getData());
}
