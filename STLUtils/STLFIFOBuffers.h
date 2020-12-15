#pragma once

#include <mutex>
#include <map>
#include "STLByteArray.h"


using namespace std;

/*
* �������
*  ����ͨ���ڲ���handler.handleRead()������appendBuff()��ȥ���ڲ���handler.handleNoRead()������removeBuff()���������Եõ�һ���Զ˷���������������
*/
class STLFIFOBuffers
{
public:
	STLFIFOBuffers();
	virtual ~STLFIFOBuffers();

public:
	bool appendBuff(int fd, const void* pData, int nSize);
	bool removeBuff(int fd, STLByteArray& buff);
	bool queryBuff(int fd, int& size);

private:
	/**
	 * ����
	 */
	mutex							lock;

	/**
	 * ÿ��fd��Ӧ�Ļ���
	 */
	map<int, STLByteArray*>			fd2buff;
};


