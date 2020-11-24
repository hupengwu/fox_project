#pragma once

#include <netinet/in.h>

/*
* �ͻ���socket key��Ϣ
*/
class FoxTcpSocketKey
{
public:
	FoxTcpSocketKey();
	FoxTcpSocketKey(const FoxTcpSocketKey& src);
	virtual ~FoxTcpSocketKey();

	const FoxTcpSocketKey& operator=(const FoxTcpSocketKey& dateSrc);

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
	int			writeSocket(const char* buff,int length);

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

