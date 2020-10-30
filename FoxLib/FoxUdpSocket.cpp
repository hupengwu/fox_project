#include "FoxUdpSocket.h"


FoxUdpSocket::FoxUdpSocket()
{
	m_hSocket = INVALID_SOCKET;
	m_hThread = INVALID_SOCKET;
	m_lEvent = FD_RDWR;
	SetThreadProc(ThreadProc);
}

FoxUdpSocket::~FoxUdpSocket()
{
	Stop();  // stop thread 
	if (IsWorking())
	{
		Exit(); // exit thread
	}
	close(m_hSocket);
}

bool FoxUdpSocket::Create(UINT nSocketPort)
{
#ifdef _DEBUG
	m_nSocketPort = nSocketPort;
#endif

	// 创建SOCKET
	m_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	// 把m_hSocket设定为非阻塞模式
	fcntl(m_hSocket, F_SETFL, O_NONBLOCK);

	struct sockaddr_in  sockAddr;
	bzero(&sockAddr, sizeof(struct sockaddr_in));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons((u_short)nSocketPort);

	if (bind(m_hSocket, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr)) < 0)
	{
		return FALSE;
	}

	// 启动SOCKET线程
	return Start();
}


void* FoxUdpSocket::ThreadProc(void* pParam)
{

	//return NULL;
	FoxUdpSocket* pObject = (FoxUdpSocket*)pParam;

#ifdef _DEBUG
	UINT* nSocketPort = &pObject->m_nSocketPort;
#endif
	//   int nCount;
   //    socklen_t len;
	//   struct sockaddr_in  cliaddr;
	//   char* lpBuf = new char[_UDPSOCKETRECVBUFMX];
	   //CString		strIP;
   //	UINT		nPort;

   // 	fd_set fdsr;
   // 	struct timeval timeout;
#ifdef _DEBUG
//	std::cout<<"Socket porc start"<<std::endl;
	Debug_Thread_Print_Other("Socket porc start");
#endif

	WAITFORSTART();
	struct timeval timeout;

	//int i=0;
	while (true)
	{
		WAITFOREVENT();

		//		std::cout<<i<<std::endl;
		//		i++;

		timeout.tv_sec = 0;
		timeout.tv_usec = 10000;

		if (pObject->Select(timeout))
		{
			if (FD_ISSET(pObject->m_hSocket, &pObject->m_rfds))
			{
				//std::cout << "recv " << std::endl;
				pObject->OnReceive(0);
			}
		}
	}

	return 0;

}

int FoxUdpSocket::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
{
	return    recvfrom(this->m_hSocket, lpBuf, nBufLen, 0, (struct sockaddr*)lpSockAddr, (socklen_t*)lpSockAddrLen);
}

int FoxUdpSocket::SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
{
	return SendTo((char*)lpBuf, nBufLen, lpSockAddr, nSockAddrLen);
}

void FoxUdpSocket::Close()
{

	if (m_hSocket == INVALID_SOCKET)
		return;

	// 	if(IsWorking())
	// 	{
	// 		Stop();  // stop thread 
	// 	}

	if (IsWorking())
	{
		Exit(); // exit thread
		Join(); // 回收SOCKET线程资源
	}

	// 通知:不再接收和发送数据
#define SD_BOTH 0x02
	shutdown(m_hSocket, SD_BOTH);

	// 关闭SOCKET:将中断recvfrom等阻塞函数
	//closesocket(m_hSocket);
	close(m_hSocket);

	m_hSocket = INVALID_SOCKET;

}

int FoxUdpSocket::SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen)
{

	if (m_hSocket == 0)
	{
		return 0;
	}
	if (!lpBuf || nBufLen <= 0)
	{
		return SOCKET_ERROR;
	}

	int dataPtr = 0;
	while (nBufLen > 0)
	{
		int sentBytes = sendto(m_hSocket, (char*)lpBuf + dataPtr, nBufLen, 0, lpSockAddr, nSockAddrLen);
		if (sentBytes == SOCKET_ERROR)
		{
			//int iError =ERRNO;// WSAGetLastError();
			return SOCKET_ERROR;
		}

		dataPtr += sentBytes;
		nBufLen -= sentBytes;
	}


	return dataPtr;
}


int FoxUdpSocket::SendTo(const void* lpBuf, int nBufLen, unsigned short wPort, const char* strIP)
{

	SOCKADDR_IN sockAddr;

	memset(&sockAddr, 0, sizeof(sockAddr));

	LPSTR lpszAscii = (char*)strIP;
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			/*
			LPHOSTENT lphost;
			lphost = gethostbyname(lpszAscii);
			if (lphost != NULL)
				sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else
			{
				//WSASetLastError(WSAEINVAL);
				return SOCKET_ERROR;
			}
			*/
			return SOCKET_ERROR; //这里肯定有bug 以后再研究
		}
	}

	sockAddr.sin_port = htons((u_short)wPort);

	return SendTo(lpBuf, nBufLen, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

}

void FoxUdpSocket::OnReceive(int errorcode)  //base is empty
{

}

BOOL FoxUdpSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)//hhc-2009-11-4
{
	if (::setsockopt(m_hSocket, nLevel, nOptionName, &lpOptionValue, sizeof(lpOptionValue)) < 0)//nLevel一般为SOL_SOCKET；nOptionName：SO_SNDBUF 或SO_RCVBUF；lpOptionValue：要设置的缓冲大小,1M为1024*1024
		return FALSE;
	return TRUE;
}


BOOL FoxUdpSocket::Create(UINT nSocketPort, int nSocketType)
{
#ifdef _DEBUG
	m_nSocketPort = nSocketPort;
#endif
	// struct sockaddr_in *addr;

	// 创建SOCKET
	m_hSocket = socket(AF_INET, nSocketType, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	// 把m_hSocket设定为非阻塞模式
	fcntl(m_hSocket, F_SETFL, O_NONBLOCK);

	struct sockaddr_in sockAddr;
	bzero(&sockAddr, sizeof(struct sockaddr_in));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons((u_short)nSocketPort);

	if (bind(m_hSocket, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr)) < 0)
	{
		return FALSE;
	}

	// 启动SOCKET线程
	return Start();

}


BOOL FoxUdpSocket::Create(UINT nSocketPort, int nSocketType, LPCTSTR lpszSocketAddress)	//hhc-2009-11-4
{
#ifdef _DEBUG
	m_nSocketPort = nSocketPort;
#endif

	// struct sockaddr_in *addr=(struct sockaddr_in *)lpszSocketAddress;

	// 创建SOCKET
	m_hSocket = socket(AF_INET, nSocketType, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	// 把m_hSocket设定为非阻塞模式
	fcntl(m_hSocket, F_SETFL, O_NONBLOCK);

	// 设置SOCKEADDR
	struct sockaddr_in sockAddr;
	bzero(&sockAddr, sizeof(struct sockaddr_in));
	sockAddr.sin_family = AF_INET;
	if (lpszSocketAddress == NULL)
	{
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		DWORD lResult = inet_addr(lpszSocketAddress);
		if (lResult == INADDR_NONE)
		{
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}
	sockAddr.sin_port = htons((u_short)nSocketPort);

	if (bind(m_hSocket, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr)) < 0)
	{
		return FALSE;
	}

	// 启动SOCKET线程
	return Start();

}


BOOL FoxUdpSocket::Create(UINT nSocketPort, int nSocketType, long lEvent, LPCTSTR lpszSocketAddress)	//hhc-2009-11-4
{
#ifdef _DEBUG
	m_nSocketPort = nSocketPort;
#endif
	//struct sockaddr_in *addr=(struct sockaddr_in *)lpszSocketAddress;

	// 创建SOCKET
	m_hSocket = socket(AF_INET, nSocketType, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	// 把m_hSocket设定为非阻塞模式
	fcntl(m_hSocket, F_SETFL, O_NONBLOCK);

	// 设置SOCKEADDR
	struct sockaddr_in sockAddr;
	bzero(&sockAddr, sizeof(struct sockaddr_in));
	sockAddr.sin_family = AF_INET;
	if (lpszSocketAddress == NULL)
	{
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		DWORD lResult = inet_addr(lpszSocketAddress);
		if (lResult == INADDR_NONE)
		{
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}
	sockAddr.sin_port = htons((u_short)nSocketPort);

	// Bind
	if (bind(m_hSocket, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr)) < 0)
	{
		return FALSE;
	}

	// 设置接收事件
	m_lEvent = lEvent;

	// 启动SOCKET线程
	return Start();

}

BOOL FoxUdpSocket::AsyncSelect()
{
	//    fd_set rfds,wfds;
	//    struct timeval tv;
	//    int ret;
	// 
	//    FD_ZERO(&rfds);
	//    FD_ZERO(&wfds);
	// 
	//    FD_SET(m_hSocket,&rfds);
	//    FD_SET(m_hSocket,&wfds);
	// 
	//    tv.tv_sec=0;
	//    tv.tv_usec=0;
	// 
	//    if(m_hSocket==INVALID_SOCKET)
	//    {
	//      return FALSE;
	//    }
	// 
	//    ret=select(m_hSocket+1,&rfds,&wfds,NULL,&tv);
	//    if(ret>0)
	//    {
	// 	  return TRUE;
	//    }
	//    else
	//    {
	//      return FALSE;
	//    }

	return TRUE;

}
BOOL FoxUdpSocket::AsyncSelect(long lEvent)
{
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	//    fd_set rfds,wfds;
	//    struct timeval tv;
	//    int ret;
	// 
	//    FD_ZERO(&rfds);
	//    FD_ZERO(&wfds);
	// 
	//    FD_SET(m_hSocket,&rfds);
	//    FD_SET(m_hSocket,&wfds);
	// 
	//    tv.tv_sec=0;
	//    tv.tv_usec=0;
	// 
	//    ret=select(m_hSocket+1,&rfds,&wfds,NULL,&tv);
	//    if(ret>0)
	//    {
	// 	   m_lEvent=lEvent;
	// 	   switch(m_lEvent)
	// 	   {
	// 	   case FD_RD:
	// 		   return (FD_ISSET(m_hSocket,&rfds));
	// 	   case FD_WR:
	// 		   return (FD_ISSET(m_hSocket,&wfds));
	// 	   case FD_RDWR:
	// 		   return TRUE;
	// 	   }
	//    }
	// 
	//    return FALSE;

	m_lEvent = lEvent;

	return TRUE;
}

BOOL FoxUdpSocket::GetSockName(FoxString& rSocketAddress, UINT& rSocketPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	if (getsockname(m_hSocket, (SOCKADDR*)&sockAddr, (socklen_t*)&nSockAddrLen) < 0)
	{
		return FALSE;
	}
	rSocketPort = ntohs(sockAddr.sin_port);
	rSocketAddress = inet_ntoa(sockAddr.sin_addr);

	return TRUE;
}
/*
int  FoxUdpSocket::SendTo(const char* lpBuf, int nBufLen,unsigned short wPort,TString* strIP)//hhc-2009-11-4
{
	return 0;
}
 */


DWORD FoxUdpSocket::GetLastError()
{
	return errno;
}

BOOL FoxUdpSocket::Select(struct timeval tv)
{
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	FD_ZERO(&m_rfds);
	//FD_ZERO(&m_wfds);

	FD_SET(m_hSocket, &m_rfds);
	//FD_SET(m_hSocket,&m_wfds);
	//int ret = ::select(m_hSocket+1,&m_rfds,&m_wfds,NULL,&tv);

	int ret = ::select(m_hSocket + 1, &m_rfds, NULL, NULL, &tv);
	if (ret > 0)
	{
		return TRUE;
	}

	return FALSE;
}

