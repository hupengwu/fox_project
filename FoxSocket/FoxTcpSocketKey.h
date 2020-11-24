#pragma once

#include <netinet/in.h>

/*
* 客户端socket key信息
*/
class FoxTcpSocketKey
{
public:
	FoxTcpSocketKey();
	FoxTcpSocketKey(const FoxTcpSocketKey& src);
	virtual ~FoxTcpSocketKey();

	const FoxTcpSocketKey& operator=(const FoxTcpSocketKey& dateSrc);

public:// 属性
	/*
	* 客户端地址
	*/
	sockaddr_in getSocketAddr();
	void		setSocketAddr(sockaddr_in sockaddr);

	/*
	* socket句柄
	*/
	int			getSocket();
	void		setSocket(int hClientSocket);

	/*
	* 主动失效标识
	*/
	void		setInvalid(bool invalid);
	bool		getInvalid();

public:// socket操作
	int			writeSocket(const char* buff,int length);

private:
	/*
	* 是否失效
	*/
	bool		invalid;

	/*
	* socket句柄
	*/
	int			socket;

	/*
	* 客户端地址
	*/
	sockaddr_in sockaddr;
};

