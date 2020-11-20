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

    

    //����һ��socket��ַ�ṹserverAddr,���������internet��ַ, �˿�
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr)); //��һ���ڴ���������ȫ������Ϊ0
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);
    serverAddr.sin_port = htons(socketPort);

    //��������internet����Э��(TCP)socket,��server_socket���������socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serverSocket)
    {
        //logger->info("Create Socket Failed!");
        return false;
    }
  //  logger->info("socket create successfully!");

    //��socket��socket��ַ�ṹ��
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)))
    {
     //   std::string message = STLStringUtils::snprintf("Bind error.IP[%s], Port[%d]", serverIp, serverAddr.sin_port);
    //    logger->info(message);
        return false;
    }

    //server_socket���ڼ���
    if (listen(serverSocket, 20))
    {
   //     logger->info("Listen error!");
        return false;
    }
  //  logger->info(STLStringUtils::snprintf("Listening on port[% d]", serverAddr.sin_port));

    while (1) //��������Ҫһֱ����
    {
        //����ͻ��˵�socket��ַ�ṹclientAddr
        struct sockaddr_in clientAddr;
        int ilength = sizeof(clientAddr);

        //����һ����serverSocket�����socket��һ������
        //���û����������,�͵ȴ�������������--����accept����������(����)
        //accept��������һ���µ�socket,���socket(new_server_socket)����ͬ���ӵ��Ŀͻ���ͨ��
        //new_server_socket�����˷������Ϳͻ���֮���һ��ͨ��ͨ��
        //accept���������ӵ��Ŀͻ�����Ϣ��д���ͻ��˵�socket��ַ�ṹclientAddr��

        int new_server_socket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)(&ilength));
        if (-1 == new_server_socket)
        {
            printf("Server Accept Failed!\n");
            break;
        }
        void* threadReturn;
    //    pthread_t child_thread;

        // �½�һ���߳�
        // �ڶ�������ΪNULL��Ĭ��ΪPTHREAD_CREATE_JOINABLE״̬����Ҫ�ֶ��ͷ��߳���Դ
    //    if (pthread_create(&child_thread, NULL, talk_to_client, (void*)new_server_socket) < 0)
    //    {
    //        printf("pthread_create Failed : %s\n", strerror(errno));
    //    }
        printf("Create Thread Success!\n");
    }
    //�رռ����õ�socket
    close(serverSocket);

	return true;
}
