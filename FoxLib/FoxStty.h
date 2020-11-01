#pragma once
#include <pthread.h> 
#include <termios.h> 

// 串口设备信息结构 
typedef struct tty_info_t
{
    int fd; // 串口设备ID 
    pthread_mutex_t mt; // 线程同步互斥对象 
    char name[24]; // 串口设备名称，例："/dev/ttyS0" 
    struct termios ntm; // 新的串口设备选项 
    struct termios otm; // 旧的串口设备选项 
} TTY_INFO;

/*从头文件中的函数定义不难看出，函数的功能，使用过程如下：
（1） 打开串口设备，调用函数setTTYSpeed（）；
（2） 设置串口读写的波特率，调用函数setTTYSpeed（）；
（3） 设置串口的属性，包括停止位、校验位、数据位等，调用函数setTTYParity（）；
（4） 向串口写入数据，调用函数sendnTTY（）；
（5） 从串口读出数据，调用函数recvnTTY（）；
（6） 操作完成后，需要调用函数cleanTTY（）来释放申请的串口信息接口；
其中，lockTTY（）和unlockTTY（）是为了能够在多线程中使用。在读写操作的前后，需要锁定和释放串口资源。
具体的使用方法，在代码实现的原文件中，main（）函数中进行了演示。下面就是源代码文件： */

enum TTYParity { N, E, O, S };

class FoxStty
{
public:
    // 初始化串口设备并进行原有设置的保存 
    static TTY_INFO* readyTTY(int id);

    // 设置串口通信速率 
    static int setTTYSpeed(TTY_INFO* ptty, int speed);

    // 设置串口数据位，停止位和效验位
    static int setTTYParity(TTY_INFO* ptty, int databits, TTYParity parity, int stopbits);

    // 清理串口设备资源 
    static int cleanTTY(TTY_INFO* ptty);

    // 发送串口数据
    static int sendnTTY(TTY_INFO* ptty, char* pbuf, int size);

    // 读取串口数据
    static int recvnTTY(TTY_INFO* ptty, char* pbuf, int size);

public:
    // 锁定串口
    static int lockTTY(TTY_INFO* ptty);

    // 解锁串口
    static int unlockTTY(TTY_INFO* ptty);

public:
    // 锁定串口
    static int demo();
};

