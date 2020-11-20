#include "FoxTcpServerSocket.h"

#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <arpa/inet.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <pthread.h>
#include <sys/errno.h>    // for errno
#include <unistd.h>   //define close

//#include <FoxLoggerFactory.h>
//#include <STLStringUtils.h>

//ILogger* FoxTcpServerSocket::logger = nullptr;

bool FoxTcpServerSocket::create(const char* serverIp,unsigned int socketPort)
{
 //   FoxLoggerFactory ss;

    //getLogger();

    

    //设置一个socket地址结构serverAddr,代表服务器internet地址, 端口
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr)); //把一段内存区的内容全部设置为0
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);
    serverAddr.sin_port = htons(socketPort);

    //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serverSocket)
    {
        //logger->info("Create Socket Failed!");
        return false;
    }
  //  logger->info("socket create successfully!");

    //把socket和socket地址结构绑定
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)))
    {
     //   std::string message = STLStringUtils::snprintf("Bind error.IP[%s], Port[%d]", serverIp, serverAddr.sin_port);
    //    logger->info(message);
        return false;
    }

    //server_socket用于监听
    if (listen(serverSocket, 20))
    {
   //     logger->info("Listen error!");
        return false;
    }
  //  logger->info(STLStringUtils::snprintf("Listening on port[% d]", serverAddr.sin_port));

    while (1) //服务器端要一直运行
    {
        //定义客户端的socket地址结构clientAddr
        struct sockaddr_in clientAddr;
        int ilength = sizeof(clientAddr);

        //接受一个到serverSocket代表的socket的一个连接
        //如果没有连接请求,就等待到有连接请求--这是accept函数的特性(阻塞)
        //accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信
        //new_server_socket代表了服务器和客户端之间的一个通信通道
        //accept函数把连接到的客户端信息填写到客户端的socket地址结构clientAddr中

        int new_server_socket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)(&ilength));
        if (-1 == new_server_socket)
        {
            printf("Server Accept Failed!\n");
            break;
        }
        void* threadReturn;
    //    pthread_t child_thread;

        // 新建一个线程
        // 第二个参数为NULL则默认为PTHREAD_CREATE_JOINABLE状态，需要手动释放线程资源
    //    if (pthread_create(&child_thread, NULL, talk_to_client, (void*)new_server_socket) < 0)
    //    {
    //        printf("pthread_create Failed : %s\n", strerror(errno));
    //    }
        printf("Create Thread Success!\n");
    }
    //关闭监听用的socket
    close(serverSocket);

	return true;
}
