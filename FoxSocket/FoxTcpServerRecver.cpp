#include "FoxTcpServerRecver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <FoxLoggerFactory.h>
#include <STLStringUtils.h>

#include "FoxTcpSocketKey.h"

ILogger* FoxTcpServerRecver::logger = FoxLoggerFactory::getLogger();

constexpr auto BUFF_SIZE_MAX = 16*1024;

void FoxTcpServerRecver::run()
{
    char recvBuff[BUFF_SIZE_MAX];

    FoxTcpSocketHandler& handler    = *this->socketHandler;
    FoxTcpSocketKey& key            = this->socketKey;
    int socket                      = key.getSocket();

    // <1> �ͻ������ӽ���
    logger->info("connect from client, address : %s, port : %d ,Socket Num : % d",
        inet_ntoa(key.getSocketAddr().sin_addr),
        key.getSocketAddr().sin_port,
        socket);
    handler.handleConnect(key);

    while (true)
    {
        // ���տͻ��˷���������Ϣ��buffer��
        int length = ::recv(socket, recvBuff, BUFF_SIZE_MAX, 0);

        // <2> ���յ����ݣ�����0��
        if (length > 0)
        {
            handler.handleRead(key, recvBuff, length);
        }

        // <3> ���յ��ͻ��˶Ͽ�����Ϣ������0������ ����������Ͽ��ÿͻ������� ���� �����socketͨ��handler֪ͨ�������˳�����
        if ((length == 0) || key.getInvalid() || handler.getExit())
        {
            logger->info("disconnect from client, address : %s, port : %d ,Socket Num : % d",
                inet_ntoa(key.getSocketAddr().sin_addr),
                key.getSocketAddr().sin_port,
                socket);

            handler.handleDisconnect(key);
            break;
        }
    }

    // �ر�socket
    ::shutdown(socket, 0x02);
    ::close(socket);

    return;
}

FoxTcpServerRecver::FoxTcpServerRecver(FoxTcpSocketKey& socketKey, FoxTcpSocketHandler* socketHandler)
{
    this->socketKey = socketKey;
    this->socketHandler = socketHandler;
}

FoxTcpServerRecver::~FoxTcpServerRecver()
{
}

