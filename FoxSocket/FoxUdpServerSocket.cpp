#include "FoxUdpServerSocket.h"

#include <FoxLoggerFactory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

FoxUdpServerSocket::FoxUdpServerSocket()
{
}

FoxUdpServerSocket::~FoxUdpServerSocket()
{
}

bool FoxUdpServerSocket::create(int serverPort)
{
    // <1> 创建socket
    int serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> 设置recv超时:1秒
    struct timeval timeout = { 1,0 };
    if (setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }

    // <3> 绑定socket句柄和地址+端口
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((u_short)serverPort);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        logger->info("Bind error.Port[%d]", serverAddr.sin_port);
        return false;
    }
    this->socketKey.setSocketAddr(serverAddr);

    // <4> 保存socket信息
    this->socketKey.setSocket(serverSocket);
    this->socketKey.setSocketAddr(serverAddr);
    this->socketKey.setInvalid(false);

    // <5> 启动一个专门收发的线程
    FoxUdpServerSocket* socket = this;
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, socket);

    return true;
}

int FoxUdpServerSocket::sendTo(const char* buff, int buffLen, sockaddr_in& addr_client,int addrLen)
{
    int socket = this->socketKey.getSocket();
    return ::sendto(socket, buff, buffLen, 0, (struct sockaddr*)&addr_client, addrLen);
}

void FoxUdpServerSocket::close()
{
    // 通知handler退出：handler处理完毕后，关闭客户端的socket
    this->socketHandler->setExit(true);

    // 通知监听线程退出
    this->setExit(true);

    // 检查：全体线程是否运行结束
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    // 回收线程
    thread* thread = this->recvThread;
    if (thread != nullptr)
    {
        if (thread->joinable())
        {
            thread->join();
        }
        delete this->recvThread;
        this->recvThread = nullptr;
    }


    // 重置标识
    this->setFinished(true);
    this->setExit(false);
    this->socketHandler->setExit(false);

    // 关闭本地socket
    int socket = this->socketKey.getSocket();
    if (socket != -1)
    {
        this->socketHandler->handleClosed(this->socketKey);

        ::shutdown(socket, 0x02);
        ::close(socket);
        this->socketKey.setSocket(-1);
    }
}

void FoxUdpServerSocket::recvFunc(FoxSocket* socket)
{
    FoxUdpServerSocket* serverSocket = (FoxUdpServerSocket*)socket;
    FoxSocketHandler& handler = *serverSocket->socketHandler;
    FoxSocketKey& serverKey = serverSocket->socketKey;

    struct sockaddr_in      addr_client;
    int                     addr_len;


    // <1> 接收到客户端发过来的消息       
    int length = recvfrom(serverKey.getSocket(), recvBuff, sizeof(recvBuff), 0, (struct sockaddr*)&addr_client, (socklen_t*)&addr_len);
    if (length < 0)
    {
        return;
    }

    // <2> 接收到了客户端发送过来的数据（大于或等于0）
    if (length >= 0)
    {
        handler.handleReadFrom(serverKey, recvBuff, length, addr_client, addr_len);
        return;
    }
}
