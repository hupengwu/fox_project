#include "FoxStty.h"

#include <stdio.h> 
#include <string.h>
#include <sys/ioctl.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/file.h>

//#include <sys/types.h> 
//#include <sys/stat.h> 
//#include <errno.h> 

/*
TTY_INFO* FoxStty::readyTTY(int id)
{
    return nullptr;
}

int FoxStty::setTTYSpeed(TTY_INFO* ptty, int speed)
{
    return 0;
}

int FoxStty::setTTYParity(TTY_INFO* ptty, int databits, int parity, int stopbits)
{
    return 0;
}

int FoxStty::cleanTTY(TTY_INFO* ptty)
{
    return 0;
}

int FoxStty::sendnTTY(TTY_INFO* ptty, char* pbuf, int size)
{
    return 0;
}

int FoxStty::recvnTTY(TTY_INFO* ptty, char* pbuf, int size)
{
    return 0;
}

int FoxStty::lockTTY(TTY_INFO* ptty)
{
    return 0;
}

int FoxStty::unlockTTY(TTY_INFO* ptty)
{
    return 0;
}

*/

/// 
// ��ʼ�������豸������ԭ�����õı��� 
TTY_INFO* FoxStty::readyTTY(int id)
{
    TTY_INFO* ptty;

    ptty = (TTY_INFO*)malloc(sizeof(TTY_INFO));
    if (ptty == NULL)
        return NULL;
    memset(ptty, 0, sizeof(TTY_INFO));
    pthread_mutex_init(&ptty->mt, NULL);
    sprintf(ptty->name, "/dev/ttyS%d", id);
    // 
    // �򿪲������ô��� 
    ptty->fd = open(ptty->name, O_RDWR | O_NOCTTY | O_NDELAY);
    if (ptty->fd < 0)
    {
        free(ptty);
        return NULL;
    }
    // 
    // ȡ�ò��ұ���ԭ�������� 
    tcgetattr(ptty->fd, &ptty->otm);
    return ptty;
}

/// 
// �������豸��Դ 
int FoxStty::cleanTTY(TTY_INFO* ptty)
{
    // 
    // �رմ򿪵Ĵ����豸 
    if (ptty->fd > 0)
    {
        tcsetattr(ptty->fd, TCSANOW, &ptty->otm);
        close(ptty->fd);
        ptty->fd = -1;
        free(ptty);
        ptty = NULL;
    }

    return 0;
}


/// 
// ���ô���ͨ������ 
// ptty ��������(TTY_INFO *),�Ѿ���ʼ���Ĵ����豸��Ϣ�ṹָ�� 
// speed ��������(int),�������ô��ڵĲ����� 
// return ����ֵ����(int),����ִ�гɹ�������ֵ�����򷵻ش������ֵ 
/// 
int FoxStty::setTTYSpeed(TTY_INFO* ptty, int speed)
{
    int i;
    // 
    // �����µĴ�������,����λΪ8λ 
    bzero(&ptty->ntm, sizeof(ptty->ntm));
    tcgetattr(ptty->fd, &ptty->ntm);
    ptty->ntm.c_cflag = /*CS8 |*/ CLOCAL | CREAD;

    switch (speed)
    {
    case 300:
        ptty->ntm.c_cflag |= B300;
        break;
    case 1200:
        ptty->ntm.c_cflag |= B1200;
        break;
    case 2400:
        ptty->ntm.c_cflag |= B2400;
        break;
    case 4800:
        ptty->ntm.c_cflag |= B4800;
        break;
    case 9600:
        ptty->ntm.c_cflag |= B9600;
        break;
    case 19200:
        ptty->ntm.c_cflag |= B19200;
        break;
    case 38400:
        ptty->ntm.c_cflag |= B38400;
        break;
    case 115200:
        ptty->ntm.c_cflag |= B115200;
        break;
    }
    ptty->ntm.c_iflag = IGNPAR;
    ptty->ntm.c_oflag = 0;
    // 
    // 
    tcflush(ptty->fd, TCIFLUSH);
    tcsetattr(ptty->fd, TCSANOW, &ptty->ntm);
    // 
    // 
    return 0;
}
// ���ô�������λ��ֹͣλ��Ч��λ 
// ptty ��������(TTY_INFO *),�Ѿ���ʼ���Ĵ����豸��Ϣ�ṹָ�� 
// databits ��������(int), ����λ,ȡֵΪ7����8 
// stopbits ��������(int),ֹͣλ,ȡֵΪ1����2 
// parity ��������(int),Ч������ ȡֵΪN,E,O,,S 
// return ����ֵ����(int),����ִ�гɹ�������ֵ�����򷵻ش������ֵ 
/// 
int FoxStty::setTTYParity(TTY_INFO* ptty, int databits, Parity parity, int stopbits)
{
    // 
    // ȡ�ô������� 
    if (tcgetattr(ptty->fd, &ptty->ntm) != 0)
    {
        printf("SetupSerial [%s]\n", ptty->name);
        return 1;
    }

    bzero(&ptty->ntm, sizeof(ptty->ntm));
    ptty->ntm.c_cflag = CS8 | CLOCAL | CREAD;
    ptty->ntm.c_iflag = IGNPAR;
    ptty->ntm.c_oflag = 0;
    // 
    // ���ô��ڵĸ��ֲ��� 
    ptty->ntm.c_cflag &= ~CSIZE;
    switch (databits)
    { //��������λ�� 
    case 7:
        ptty->ntm.c_cflag |= CS7;
        break;
    case 8:
        ptty->ntm.c_cflag |= CS8;
        break;
    default:
        printf("Unsupported data size\n");
        return 5;
    }
    // 
    // 
    switch (parity)
    { // ������żУ��λ�� 
    case N:
        ptty->ntm.c_cflag &= ~PARENB; /* Clear parity enable */
        ptty->ntm.c_iflag &= ~INPCK; /* Enable parity checking */
        break;
    case O:
        ptty->ntm.c_cflag |= (PARODD | PARENB); /* ����Ϊ��Ч��*/
        ptty->ntm.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case E:
        ptty->ntm.c_cflag |= PARENB; /* Enable parity */
        ptty->ntm.c_cflag &= ~PARODD; /* ת��ΪżЧ��*/
        ptty->ntm.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case S: /*as no parity*/
        ptty->ntm.c_cflag &= ~PARENB;
        ptty->ntm.c_cflag &= ~CSTOPB;
        break;
    default:
        printf("Unsupported parity\n");
        return 2;
    }
    // 
    // ����ֹͣλ 
    switch (stopbits)
    {
    case 1:
        ptty->ntm.c_cflag &= ~CSTOPB;
        break;
    case 2:
        ptty->ntm.c_cflag |= CSTOPB;
        break;
    default:
        printf("Unsupported stop bits\n");
        return 3;
    }
    // 
    // 
    ptty->ntm.c_lflag = 0;
    ptty->ntm.c_cc[VTIME] = 0; // inter-character timer unused 
    ptty->ntm.c_cc[VMIN] = 1; // blocking read until 1 chars received 
    tcflush(ptty->fd, TCIFLUSH);
    if (tcsetattr(ptty->fd, TCSANOW, &ptty->ntm) != 0)
    {
        printf("SetupSerial \n");
        return 4;
    }

    return 0;
}

int FoxStty::recvnTTY(TTY_INFO* ptty, char* pbuf, int size)
{
    int ret, left, bytes;

    left = size;

    while (left > 0)
    {
        ret = 0;
        bytes = 0;

        pthread_mutex_lock(&ptty->mt);
        ioctl(ptty->fd, FIONREAD, &bytes);
        if (bytes > 0)
        {
            ret = read(ptty->fd, pbuf, left);
        }
        pthread_mutex_unlock(&ptty->mt);
        if (ret > 0)
        {
            left -= ret;
            pbuf += ret;
        }
        usleep(100);
    }

    return size - left;
}

int FoxStty::sendnTTY(TTY_INFO* ptty, char* pbuf, int size)
{
    int ret, nleft;
    char* ptmp;

    ret = 0;
    nleft = size;
    ptmp = pbuf;

    while (nleft > 0)
    {
        pthread_mutex_lock(&ptty->mt);
        ret = write(ptty->fd, ptmp, nleft);
        pthread_mutex_unlock(&ptty->mt);

        if (ret > 0)
        {
            nleft -= ret;
            ptmp += ret;
        }
        //usleep(100); 
    }

    return size - nleft;
}

int FoxStty::lockTTY(TTY_INFO* ptty)
{
    if (ptty->fd < 0)
    {
        return 1;
    }

    return flock(ptty->fd, LOCK_EX);
}
int FoxStty::unlockTTY(TTY_INFO* ptty)
{
    if (ptty->fd < 0)
    {
        return 1;
    }

    return flock(ptty->fd, LOCK_UN);
}


// �ӿڲ��� 
int FoxStty::demo()
{
    TTY_INFO* ptty;
    int nbyte, idx;
    char cc[16];

    ptty = readyTTY(0);
    if (ptty == NULL)
    {
        printf("readyTTY(0) error\n");
        return 1;
    }

    lockTTY(ptty);
    if (setTTYSpeed(ptty, 9600) > 0)
    {
        printf("setTTYSpeed() error\n");
        return -1;
    }
    if (setTTYParity(ptty, 8, N, 1) > 0)
    {
        printf("setTTYParity() error\n");
        return -1;
    }
    // 
    idx = 0;
    while (1)
    {
        cc[0] = 0xFA;
        sendnTTY(ptty, &cc[0], 1);
        nbyte = recvnTTY(ptty, cc, 1);
        printf("%d:%02X\n", idx++, cc[0]);
    }

    cleanTTY(ptty);

}
