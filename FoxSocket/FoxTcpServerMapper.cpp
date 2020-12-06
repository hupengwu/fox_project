#include "FoxTcpServerMapper.h"

FoxTcpServerMapper::FoxTcpServerMapper()
{
}

FoxTcpServerMapper::~FoxTcpServerMapper()
{
	// 是否内存
	for (map<int, list<STLByteArray*>>::iterator itMap = this->socket2bufflist.begin(); itMap != this->socket2bufflist.end(); itMap++)
	{
		list<STLByteArray*> buffList = itMap->second;

		for (list<STLByteArray*>::iterator it = buffList.begin(); it != buffList.end(); it++)
		{
			STLByteArray* buff = *it;
			delete buff;
		}
	}
}

void FoxTcpServerMapper::addSocket(int hClientSocket)
{
	lock_guard<mutex> guard(this->lock);

	// 检查：是否已经存在该元素
	if (this->socket2bufflist.find(hClientSocket) != this->socket2bufflist.end())
	{
		return;
	}

	list<STLByteArray*> buffList;
	this->socket2bufflist[hClientSocket] = buffList;
}

void FoxTcpServerMapper::delSocket(int hClientSocket)
{
	lock_guard<mutex> guard(this->lock);

	// 检查：是否已经存在该元素
	if (this->socket2bufflist.find(hClientSocket) == this->socket2bufflist.end())
	{
		return;
	}

	// 是否内存
	list<STLByteArray*>& buffList = this->socket2bufflist[hClientSocket];
	for (list<STLByteArray*>::iterator it = buffList.begin(); it != buffList.end(); it++)
	{
		STLByteArray* buff = *it;
		delete buff;
	}

	this->socket2bufflist.erase(hClientSocket);
}

bool FoxTcpServerMapper::hasSocket(int hClientSocket)
{
	lock_guard<mutex> guard(this->lock);
	return this->socket2bufflist.find(hClientSocket) != this->socket2bufflist.end();
}

void FoxTcpServerMapper::pushBuff(int hClientSocket, STLByteArray& buff)
{
	lock_guard<mutex> guard(this->lock);	
	list<STLByteArray*>& buffList = this->socket2bufflist[hClientSocket];

	// 将外部的缓存交换过来，避免开销很大的复制过程
	STLByteArray* temp = new STLByteArray();
	temp->swap(buff);

	buffList.push_back(temp);
}

void FoxTcpServerMapper::popBuff(int hClientSocket, list<STLByteArray>& buffs)
{
	lock_guard<mutex> guard(this->lock);
	list<STLByteArray*>& buffList = this->socket2bufflist[hClientSocket];

	buffs.clear();

	for (list<STLByteArray*>::iterator it = buffList.begin(); it != buffList.end(); it++)
	{		
		STLByteArray* buff = *it;

		// 将每个内部数据交换到外部，避免不必要的拷贝
		STLByteArray* temp = new STLByteArray();
		temp->swap(*buff);
		buffs.push_back(*temp);

		// 删除已经不再使用的内部对象
		delete buff;
	}
	
	buffList.clear();
}

bool FoxTcpServerMapper::queryBuff(int hClientSocket)
{
	lock_guard<mutex> guard(this->lock);

	// 检查：是否已经存在该元素
	if (this->socket2bufflist.find(hClientSocket) == this->socket2bufflist.end())
	{
		return false;
	}

	list<STLByteArray*>& buffList = this->socket2bufflist[hClientSocket];

	// 检查：是否已经存在该元素
	return !buffList.empty();
}
