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

public:// �Ƿ����socket
	void addSocket(int hClientSocket);
	void delSocket(int hClientSocket);
	bool hasSocket(int hClientSocket);

public:// �Ƿ����socket
	void pushBuff(int hClientSocket, STLByteArray& buff);
	void popBuff(int hClientSocket, list<STLByteArray>& buffs);
	bool hasBuff(int hClientSocket);

private:
	/**
	 * ����
	 */
	mutex							lock;

	/**
	 * ÿ��socket��Ӧ�Ļ���
	 */
	map<int,list<STLByteArray*>>	socket2bufflist;

};

