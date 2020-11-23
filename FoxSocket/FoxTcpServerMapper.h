#pragma once

#include <map>
#include <list>
#include <mutex>

#include <STLByteArray.h>

using namespace std;

class FoxTcpServerMapper
{
public:
	FoxTcpServerMapper();
	virtual ~FoxTcpServerMapper();

public:// 是否存在socket
	void addSocket(int hClientSocket);
	void delSocket(int hClientSocket);
	bool hasSocket(int hClientSocket);

public:// 是否存在socket
	void pushBuff(int hClientSocket, STLByteArray& buff);
	void popBuff(int hClientSocket, list<STLByteArray>& buffs);
	bool hasBuff(int hClientSocket);

private:
	/**
	 * 互斥
	 */
	mutex							lock;

	/**
	 * 每个socket对应的缓存
	 */
	map<int,list<STLByteArray*>>	socket2bufflist;

};

