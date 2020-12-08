#pragma once

#include <mutex>
#include <map>
#include <STLByteArray.h>


using namespace std;

/*
* 缓存管理：
*  可以通过在捕获handler.handleRead()将数据appendBuff()进去，在捕获handler.handleNoRead()将数据removeBuff()出来，可以得到一个对端发过来的连续数据
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
	 * 互斥
	 */
	mutex							lock;

	/**
	 * 每个socket对应的缓存
	 */
	map<int, STLByteArray*>			fd2buff;
};

