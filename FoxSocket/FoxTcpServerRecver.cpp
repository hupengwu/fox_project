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
    FoxSocketKey& key            = this->socketKey;
    int clientSocket             = key.getSocket();

    // <1> �ͻ������ӽ���
    logger->info("connect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        clientSocket);
    handler.handleConnect(key);

    while (true)
    {
        // <2> ���տͻ��˷���������Ϣ��buffer��
        int length = ::recv(clientSocket, recvBuff, sizeof(recvBuff), 0);

        // <3> ���յ����ݣ�����0��
        if (length > 0)
        {
            handler.handleRead(key, recvBuff, length);
        }

        // <4> ���յ��ͻ��˶Ͽ�����Ϣ������0������ ����������Ͽ��ÿͻ������� ���� �����socketͨ��handler֪ͨ�������˳�����
        if ((length <= 0) || key.getInvalid() || handler.getExit())
        {            
            break;
        }
    }

    // ֪ͨ�ͻ������ӶϿ�
    logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        clientSocket);
    handler.handleDisconnect(key);

    // �ر�socket
    ::shutdown(clientSocket, 0x02);
    ::close(clientSocket);
    key.setSocket(-1);

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

