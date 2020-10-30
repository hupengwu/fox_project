#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>


#include "FoxPThreadModule.h"
#include "FoxPThreadMutex.h"
#include "FoxString.h"

typedef sockaddr  	SOCKADDR;
typedef sockaddr_in SOCKADDR_IN;
typedef int  		SOCKET;
#define SOCKET_ERROR -1
#define   INVALID_SOCKET     (SOCKET)(~0)
typedef char* LPSTR;
; //hhc-2009-11-4
#define FD_READ 1

#ifndef INADDR_NONE
#define INADDR_NONE             0xffffffff
#endif

//#define _UDPSOCKETRECVBUFMX	   2048

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class FoxUdpSocket :public FoxPThreadModule
{
public:
	enum EEvent
	{
		FD_RD = 1,
		FD_WR = 2,
		FD_RDWR = 4,
	};
public:
	virtual bool Create(UINT nSocketPort);

	virtual void OnReceive(int errorcode);

public:
	void Close();

	BOOL Create(UINT nSocketPort, int nSocketType, long lEvent, LPCTSTR lpszSocketAddress = NULL);
	BOOL Create(UINT nSocketPort, int nSocketType, LPCTSTR lpszSocketAddress);
	BOOL Create(UINT nSocketPort, int nSocketType);

	int ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = 0);
	virtual int SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags);

	int	 SendTo(const void* lpBuf, int nBufLen, unsigned short wPort, const char* strIP);
	int	 SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen);
	BOOL AsyncSelect();
	BOOL AsyncSelect(long lEvent);

	BOOL Select(struct timeval tv);


	BOOL GetSockName(FoxString& rSocketAddress, UINT& nSocketPort);
	BOOL SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET);
	//int  SendTo(const char* lpBuf, int nBufLen,unsigned short wPort,TString* strIP);
	//m_pSocket->SendTo(ctrlUDPBuff->arrData.GetData(),ctrlUDPBuff->arrData.GetSize(),ctrlUDPBuff->ctrIPPORT.wPort,strIP);

	static DWORD GetLastError();
public:
	FoxUdpSocket();
	virtual ~FoxUdpSocket();

private:
	enum ThreadState { CLOSED = 0, CLOSING, RUNNING };
	static void* ThreadProc(void* pParam);

protected:
	SOCKET				m_hSocket;
	SOCKADDR_IN			m_Address;
	int					m_hThread;

	FoxPThreadMutex		m_Lock;
private:
	long              m_lEvent;
	fd_set			  m_rfds;
	fd_set            m_wfds;

#ifdef _DEBUG
	UINT m_nSocketPort;
#endif
};
