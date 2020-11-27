#include "FoxTcpServerRecver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <FoxLoggerFactory.h>
#include <STLStringUtils.h>

#include "FoxSocketKey.h"

ILogger* FoxTcpServerRecver::logger = FoxLoggerFactory::getLogger();

void FoxTcpServerRecver::run()
{
    FoxSocketHandler& handler    = *this->socketHandler;
    FoxSocketKey& remoteKey      = this->socketKey;
    int remoteSocket             = remoteKey.getSocket();

    // <1> �ͻ������ӽ���
    logger->info("connect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(remoteKey.getSocketAddr().sin_addr),
        remoteKey.getSocketAddr().sin_port,
        remoteSocket);
    handler.handleConnect(remoteKey);

    while (true)
    {
        // <2> ���տͻ��˷���������Ϣ��buffer��
        int length = ::recv(remoteSocket, recvBuff, sizeof(recvBuff), 0);

        // <3> ���յ����ݣ�����0��
        if (length > 0)
        {
            handler.handleRead(remoteKey, recvBuff, length);
        }

        // <4> ���յ��ͻ��˶Ͽ�����Ϣ������0������ ����������Ͽ��ÿͻ������� ���� �����socketͨ��handler֪ͨ�������˳�����
        if ((length <= 0) || remoteKey.getInvalid() || handler.getExit())
        {            
            break;
        }
    }

    // ֪ͨ�ͻ������ӶϿ�
    logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(remoteKey.getSocketAddr().sin_addr),
        remoteKey.getSocketAddr().sin_port,
        remoteSocket);
    handler.handleDisconnect(remoteKey);

    // �ر�socket
    ::shutdown(remoteSocket, 0x02);
    ::close(remoteSocket);
    remoteKey.setSocket(-1);

    return;
}

FoxTcpServerRecver::FoxTcpServerRecver(FoxSocketKey& socketKey, FoxSocketHandler* socketHandler)
{
    this->socketKey = socketKey;
    this->socketHandler = socketHandler;
}

FoxTcpServerRecver::~FoxTcpServerRecver()
{
}

