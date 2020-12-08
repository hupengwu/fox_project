#pragma once

#include <mutex>
#include <map>
#include <STLByteArray.h>


using namespace std;

/*
* �������
*  ����ͨ���ڲ���handler.handleRead()������appendBuff()��ȥ���ڲ���handler.handleNoRead()������removeBuff()���������Եõ�һ���Զ˷���������������
*/
class FoxSttyBuffer
{
public:
	FoxSttyBuffer();
	virtual ~FoxSttyBuffer();

public:
	bool appendBuff(int fd, const char* pData, int nSize);
	bool removeBuff(int fd, STLByteArray& buff);
	bool queryBuff(int fd,  int& size);

private:
	/**
	 * ����
	 */
	mutex							lock;

	/**
	 * ÿ��socket��Ӧ�Ļ���
	 */
	map<int, STLByteArray*>			fd2buff;
};

