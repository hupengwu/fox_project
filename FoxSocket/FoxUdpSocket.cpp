#include "FoxUdpSocket.h"

#include <FoxLoggerFactory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

FoxUdpSocket::FoxUdpSocket()
{
}

FoxUdpSocket::~FoxUdpSocket()
{
}

bool FoxUdpSocket::create()
{
    // <1> 创建socket
    int localSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (localSocket < 0)
    {
        logger->error("socket creation failed!");
        return false;
    }
    logger->info("socket create successfully.");

    // <2> 保存socket信息
    this->socketKey.setSocket(localSocket);
    this->socketKey.setInvalid(false);

    // <3> 设置recv超时:1秒
    struct timeval timeout = { 1,0 };
    if (::setsockopt(localSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval)) != 0)
    {
        logger->info("set recv timeout failed");
        return false;
    }            

    // <4> 启动一个专门收发的线程
    FoxUdpSocket* socket = this;
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, socket);

    return true;
}

bool FoxUdpSocket::bind(int localPort)
{
    int localSocket = this->socketKey.getSocket();
    if (localSocket <=0 || localPort <= 0)
    {
        return false;
    }

    sockaddr_in localAddr;
    bzero(&localAddr, sizeof(struct sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons((u_short)localPort);
    if (::bind(localSocket, (struct sockaddr*)&localAddr, sizeof(struct sockaddr)) < 0)
    {
        logger->info("Bind error.Port[%d]", localAddr.sin_port);
        return false;
    }
    this->socketKey.setSocketAddr(localAddr);

    return true;
}

int FoxUdpSocket::sendTo(const char* buff, int buffLen, sockaddr_in& remoteAddr,int remoteAddrLen)
{
    int localSocket = this->socketKey.getSocket();
    return ::sendto(localSocket, buff, buffLen, 0, (struct sockaddr*)&remoteAddr, remoteAddrLen);
}

int FoxUdpSocket::sendTo(const char* buff, int buffLen, const char* remoteIP, int remotePort)
{
    // 初始化地址结构
    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons((u_short)remotePort);
    remoteAddr.sin_addr.s_addr = inet_addr(remoteIP);
    int addrLen = sizeof(remoteAddr);

    int localSocket = this->socketKey.getSocket();
    return ::sendto(localSocket, buff, buffLen, 0, (struct sockaddr*)&remoteAddr, addrLen);
}

void FoxUdpSocket::close()
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

void FoxUdpSocket::recvFunc(FoxSocket* socket)
{
    FoxUdpSocket* localSocket = (FoxUdpSocket*)socket;
    FoxSocketHandler& handler = *localSocket->socketHandler;
    FoxSocketKey& localKey = localSocket->socketKey;

    struct sockaddr_in      remoteAddr{};
    int                     remoteAddrLen = 0;


    // <1> 接收到客户端发过来的消息       
    int length = recvfrom(localKey.getSocket(), recvBuff, sizeof(recvBuff), 0, (struct sockaddr*)&remoteAddr, (socklen_t*)&remoteAddrLen);
    if (length < 0)
    {
        return;
    }

    // <2> 接收到了客户端发送过来的数据（大于或等于0）
    if (length >= 0)
    {
        handler.handleReadFrom(localKey, recvBuff, length, remoteAddr, remoteAddrLen);
        return;
    }
}
