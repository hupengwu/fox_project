#pragma once

#include <netinet/in.h>

/*
* 客户端socket key信息
*/
class FoxSocketKey
{
public:
	FoxSocketKey();
	FoxSocketKey(const FoxSocketKey& src);
	virtual ~FoxSocketKey();

	const FoxSocketKey& operator=(const FoxSocketKey& dateSrc);

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
	int			send(const char* buff,int length);
	int			sendTo(const char* buff, int buffLen, sockaddr_in& addr_client, int addrLen);

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

