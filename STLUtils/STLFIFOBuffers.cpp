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

	// ��飺��������Ƿ�Ϸ�
	if (pData == nullptr || nSize <= 0) {
		return false;
	}

	// ��飺fd�Ƿ���ڣ������ھ�����һ����Ӧ��Ԫ��
	STLByteArray* data = this->fd2buff[fd];
	if (data == nullptr)
	{
		data = new STLByteArray();
		this->fd2buff[fd] = data;
	}

	// ������׷�ӵ�β��
	data->append((const char*)pData, nSize);

	return true;
}

bool STLFIFOBuffers::removeBuff(int fd, STLByteArray& buff)
{
	lock_guard<mutex> guard(this->lock);

	// ��飺�Ƿ���ڸ�Ԫ��
	map<int, STLByteArray*>::iterator it = this->fd2buff.find(fd);
	if (it == this->fd2buff.end())
	{
		return false;
	}

	STLByteArray* data = it->second;

	// �������ݳ�ȥ
	buff.swap(*data);

	// ɾ��ָ��
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

