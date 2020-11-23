#pragma once

#include <netinet/in.h>

class FoxTcpSocketKey
{
public:
	FoxTcpSocketKey();
	FoxTcpSocketKey(const FoxTcpSocketKey& src);
	virtual ~FoxTcpSocketKey();

	const FoxTcpSocketKey& operator=(const FoxTcpSocketKey& dateSrc);

public:// 属性
	sockaddr_in getSocketAddr();
	void		setSocketAddr(sockaddr_in sockaddr);

	int			getSocket();
	void		setSocket(int hClientSocket);

	void		setClosed();
	bool		getClosed();

public:// socket操作
	int			writeSocket(const char* buff,int length);

private:
	/*
	* 是否关闭连接
	*/
	bool		closed;

	/*
	* socket句柄
	*/
	int			clientSocket;

	/*
	* 客户端地址
	*/
	sockaddr_in sockaddr;
};

