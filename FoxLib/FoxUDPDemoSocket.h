#pragma once


#include "FoxUdpSocket.h"
#include "FoxPThreadAutoLock.h"

class FoxUDPDemoSocket : public FoxUdpSocket
{
	// Attributes
public:

public:
	//	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress, int nFlags);

	//	int SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags);

	// Operations
public:
	FoxUDPDemoSocket();
	virtual ~FoxUDPDemoSocket();

	// Overrides
public:
	static DWORD GetRxCount();

public:
	virtual void OnReceive(int nErrorCode);

protected:
	// 重载SendToHelper,以避免执行父类CSocket的同名函数
	virtual int SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags);

	// Implementation
protected:
	static	BYTE								m_RxBuff[4096];
	static	FoxPThreadMutex						m_Lock;
	static	SOCKADDR							m_SockAddr;

	static DWORDLONG							m_dlRxCount;
};



