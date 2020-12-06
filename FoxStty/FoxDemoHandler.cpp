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
	if (buff == nullptr || length <= 0)
	{
		return;
	}

	int size = 0;
	this->recver.queryBuff(fd, size);
	if (size > 64 * 1024)
	{

	}

	this->recver.appendBuff(fd, (char*)buff, length);

	::printf("%s\n", buff);
}

void FoxDemoHandler::handleNoRead(int fd)
{
	// 检查：是否为空数据
	int size = 0;
	if (!this->recver.queryBuff(fd, size))
	{
		return;
	}

	STLByteArray data;
	this->recver.removeBuff(fd, data);

	data.add(0);

	::printf("recv line:%s\n", data.getData());
}
