#include "STLFIFOBuffers.h"

STLFIFOBuffers::STLFIFOBuffers()
{
}

STLFIFOBuffers::~STLFIFOBuffers()
{
	lock_guard<mutex> guard(this->lock);

	for (map<int, STLByteArray*>::iterator it = this->fd2buff.begin(); it != this->fd2buff.end(); it++)
	{
		delete it->second;
	}
}

bool STLFIFOBuffers::appendBuff(int fd, const void* pData, int nSize)
{
	lock_guard<mutex> guard(this->lock);

	// 检查：输入参数是否合法
	if (pData == nullptr || nSize <= 0) {
		return false;
	}

	// 检查：fd是否存在，不存在就新增一个对应的元素
	STLByteArray* data = this->fd2buff[fd];
	if (data == nullptr)
	{
		data = new STLByteArray();
		this->fd2buff[fd] = data;
	}

	// 将数据追加到尾部
	data->append((const char*)pData, nSize);

	return true;
}

bool STLFIFOBuffers::removeBuff(int fd, STLByteArray& buff)
{
	lock_guard<mutex> guard(this->lock);

	// 检查：是否存在该元素
	map<int, STLByteArray*>::iterator it = this->fd2buff.find(fd);
	if (it == this->fd2buff.end())
	{
		return false;
	}

	STLByteArray* data = it->second;

	// 交换数据出去
	buff.swap(*data);

	// 删除指针
	delete data;

	this->fd2buff.erase(fd);
	return true;
}

bool STLFIFOBuffers::queryBuff(int fd, int& size)
{
	lock_guard<mutex> guard(this->lock);

	size = 0;
	if (this->fd2buff.find(fd) != this->fd2buff.end())
	{
		size = this->fd2buff[fd]->getSize();
		return true;
	}

	return false;
}

