#pragma once

#include <netinet/in.h>

/*
* �ͻ���socket key��Ϣ
*/
class FoxSocketKey
{
public:
	FoxSocketKey();
	FoxSocketKey(const FoxSocketKey& src);
	virtual ~FoxSocketKey();

	const FoxSocketKey& operator=(const FoxSocketKey& dateSrc);

public:// ����
	/*
	* �ͻ��˵�ַ
	*/
	sockaddr_in getSocketAddr();
	void		setSocketAddr(sockaddr_in sockaddr);

	/*
	* socket���
	*/
	int			getSocket();
	void		setSocket(int hClientSocket);

	/*
	* ����ʧЧ��ʶ
	*/
	void		setInvalid(bool invalid);
	bool		getInvalid();

public:// socket����
	int			send(const char* buff,int length);
	int			sendTo(const char* buff, int buffLen, sockaddr_in& addr_client, int addrLen);

private:
	/*
	* �Ƿ�ʧЧ
	*/
	bool		invalid;

	/*
	* socket���
	*/
	int			socket;

	/*
	* �ͻ��˵�ַ
	*/
	sockaddr_in sockaddr;
};

