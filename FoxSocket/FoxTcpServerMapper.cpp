#include "FoxTcpServerMapper.h"

FoxTcpServerMapper::FoxTcpServerMapper()
{
}

FoxTcpServerMapper::~FoxTcpServerMapper()
{
	// �Ƿ��ڴ�
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

	// ��飺�Ƿ��Ѿ����ڸ�Ԫ��
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

	// ��飺�Ƿ��Ѿ����ڸ�Ԫ��
	if (this->socket2bufflist.find(hClientSocket) == this->socket2bufflist.end())
	{
		return;
	}

	// �Ƿ��ڴ�
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

	// ���ⲿ�Ļ��潻�����������⿪���ܴ�ĸ��ƹ���
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

		// ��ÿ���ڲ����ݽ������ⲿ�����ⲻ��Ҫ�Ŀ���
		STLByteArray* temp = new STLByteArray();
		temp->swap(*buff);
		buffs.push_back(*temp);

		// ɾ���Ѿ�����ʹ�õ��ڲ�����
		delete buff;
	}
	
	buffList.clear();
}

bool FoxTcpServerMapper::queryBuff(int hClientSocket)
{
	lock_guard<mutex> guard(this->lock);

	// ��飺�Ƿ��Ѿ����ڸ�Ԫ��
	if (this->socket2bufflist.find(hClientSocket) == this->socket2bufflist.end())
	{
		return false;
	}

	list<STLByteArray*>& buffList = this->socket2bufflist[hClientSocket];

	// ��飺�Ƿ��Ѿ����ڸ�Ԫ��
	return !buffList.empty();
}
