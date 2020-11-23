#pragma once

#include <netinet/in.h>

class FoxTcpSocketKey
{
public:
	FoxTcpSocketKey();
	FoxTcpSocketKey(const FoxTcpSocketKey& src);
	virtual ~FoxTcpSocketKey();

	const FoxTcpSocketKey& operator=(const FoxTcpSocketKey& dateSrc);

public:// ����
	sockaddr_in getSocketAddr();
	void		setSocketAddr(sockaddr_in sockaddr);

	int			getSocket();
	void		setSocket(int hClientSocket);

	void		setClosed();
	bool		getClosed();

public:// socket����
	int			writeSocket(const char* buff,int length);

private:
	/*
	* �Ƿ�ر�����
	*/
	bool		closed;

	/*
	* socket���
	*/
	int			clientSocket;

	/*
	* �ͻ��˵�ַ
	*/
	sockaddr_in sockaddr;
};

