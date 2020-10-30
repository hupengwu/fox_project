#include "FoxUDPDemoSocket.h"


/////////////////////////////////////////////////////////////////////////////
// CUDPBuffClientSocket

BYTE							FoxUDPDemoSocket::m_RxBuff[4096];
FoxPThreadMutex					FoxUDPDemoSocket::m_Lock;
DWORDLONG						FoxUDPDemoSocket::m_dlRxCount = 0;
SOCKADDR						FoxUDPDemoSocket::m_SockAddr;

FoxUDPDemoSocket::FoxUDPDemoSocket()
{
}

FoxUDPDemoSocket::~FoxUDPDemoSocket()
{
}


void FoxUDPDemoSocket::OnReceive(int nErrorCode)
{
	FoxPThreadAutoLock AutoLock(m_Lock);

	FoxString		strIP;
	UINT		nPort;
	FoxString		strHead;
	char		chrHead[12];

	// 接收数据
	int			iSockAddrLen = sizeof(m_SockAddr);
	int nCount = FoxUdpSocket::ReceiveFrom(m_RxBuff, 4096, &m_SockAddr, &iSockAddrLen);
	if (nCount == SOCKET_ERROR) {
		DWORD dwError = FoxUdpSocket::GetLastError();
		FoxUdpSocket::OnReceive(nErrorCode);

		/*
		if (CBassVIParam::m_bDebugMessage)
		{
			FoxString strError;
			strError.Format("接收错误:ERROR=%d",dwError);
		}
		*/

		return;
	}
	strIP.Format("%d.%d.%d.%d",
		(BYTE)(m_SockAddr.sa_data[2]),
		(BYTE)(m_SockAddr.sa_data[3]),
		(BYTE)(m_SockAddr.sa_data[4]),
		(BYTE)(m_SockAddr.sa_data[5]));
	nPort = (BYTE)(m_SockAddr.sa_data[0]) * 0x100 + (BYTE)(m_SockAddr.sa_data[1]);

	// 合法性检查:数据长度
	if (nCount < 8)
	{
		FoxUdpSocket::OnReceive(nErrorCode);
		return;
	}

	// 获取头部:数据类型	

	memcpy(chrHead, m_RxBuff, 8);
	chrHead[8] = 0;

	strHead = chrHead;
	if (strHead == "[SERVER]")
	{
		// 添加到队列
		// 临时取消
	//	CQueueUDPClient::RecvBuff(strIP, nPort, m_RxBuff, nCount);
	}

	m_dlRxCount++;
	FoxUdpSocket::OnReceive(nErrorCode);
}

DWORD FoxUDPDemoSocket::GetRxCount()
{
	return m_dlRxCount;
}

// 函数说明:重载SendToHelper,以避免执行父类CSocket的同名函数
int FoxUDPDemoSocket::SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
{
	/*if (m_pbBlocking != NULL)
	{
		WSASetLastError(WSAEINPROGRESS);
		return SOCKET_ERROR;
	}
*/
	int nResult;
	while ((nResult = FoxUdpSocket::SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags)) == SOCKET_ERROR)
	{
		FoxUdpSocket::GetLastError();
		return SOCKET_ERROR;
	}

	return nResult;
}



