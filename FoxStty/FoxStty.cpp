#include "FoxStty.h"

FoxStty::FoxStty()
{
    this->fd = -1;

    this->uTimeOut = 100 * 1000;

    this->handler = nullptr;
    this->handler = new FoxSttyHandler();
}

FoxStty::~FoxStty()
{
    delete this->handler;
    this->handler = nullptr;
}

bool FoxStty::open(const char* name)
{
    // ���ɴ����豸������
    ::sprintf(this->name, "/dev/%s", name);

    // �򿪲������ô���
    int oflag = 0;
    oflag |= O_RDWR;// ����д��
    oflag |= O_NOCTTY;// �������豸������Ϊ�˽��̵Ŀ����ն�
    oflag |= O_NONBLOCK;// ���÷�������ʽ
    this->fd = ::open(this->name, oflag);
    if (this->fd < 0)
    {
        return false;
    }

    // ȡ�ò��ұ���ԭ�������� 
    int rtn = ::tcgetattr(this->fd, &this->otm);
    if (rtn != 0)
    {
        rtn = ::close(this->fd);
        this->fd = -1;
        return false;
    }

    // ֪ͨ�򿪴���
    this->handler->handleOpen(this->fd);

    // <6> ����һ����̨�շ����߳�
    this->createThread();

    return true;
}

bool FoxStty::isOpen()
{
    return this->fd > 0;
}

bool FoxStty::setParam(int speed, int databits, int stopbits, TTYParity parity)
{
    // ���tm����
    ::bzero(&this->ntm, sizeof(this->ntm));

    // ���ǽ��ն�����Ϊԭʼģʽ
    ::cfmakeraw(&this->ntm);

    // <1> ���ò�����
    switch (speed)
    {
    case 300:
        this->ntm.c_cflag |= B300;
        break;
    case 1200:
        this->ntm.c_cflag |= B1200;
        break;
    case 2400:
        this->ntm.c_cflag |= B2400;
        break;
    case 4800:
        this->ntm.c_cflag |= B4800;
        break;
    case 9600:
        this->ntm.c_cflag |= B9600;
        break;
    case 19200:
        this->ntm.c_cflag |= B19200;
        break;
    case 38400:
        this->ntm.c_cflag |= B38400;
        break;
    case 115200:
        this->ntm.c_cflag |= B115200;
        break;
    default:
        this->ntm.c_cflag |= B9600;
        break;
    }
    this->ntm.c_cflag = speed;
    this->ntm.c_cflag |= CLOCAL | CREAD;

    // <2> ��������λ
    this->ntm.c_cflag &= ~CSIZE;
    switch (databits)
    {
    case 5:
        this->ntm.c_cflag |= CS5;
        break;
    case 6:
        this->ntm.c_cflag |= CS6;
        break;
    case 7:
        this->ntm.c_cflag |= CS7;
        break;
    case 8:
        this->ntm.c_cflag |= CS8;
        break;
    default:
        this->ntm.c_cflag |= CS8;
        break;
    }

    // <3> ������żУ��λ�� 
    switch (parity)
    {
    case N:
        this->ntm.c_cflag &= ~PARENB; /* Clear parity enable */
        this->ntm.c_iflag &= ~INPCK; /* Enable parity checking */
        break;
    case O:
        this->ntm.c_cflag |= (PARODD | PARENB); /* ����Ϊ��Ч��*/
        this->ntm.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case E:
        this->ntm.c_cflag |= PARENB; /* Enable parity */
        this->ntm.c_cflag &= ~PARODD; /* ת��ΪżЧ��*/
        this->ntm.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case S: /*as no parity*/
        this->ntm.c_cflag &= ~PARENB;
        this->ntm.c_cflag &= ~CSTOPB;
        break;
    default:
        this->ntm.c_cflag &= ~PARENB; /* Clear parity enable */
        this->ntm.c_iflag &= ~INPCK; /* Enable parity checking */
        break;
    }

    // <4> ����ֹͣλ   
    switch (stopbits)
    {
    case 1:
        this->ntm.c_cflag &= ~CSTOPB;// 1λֹͣ
        break;
    case 2:
        this->ntm.c_cflag |= CSTOPB;// 2λֹͣλ 
        break;
    default:
        this->ntm.c_cflag &= ~CSTOPB;
        break;
    }

    // ���ÿ����ַ�
    this->ntm.c_cc[VTIME] = 1;  // ��ȡ�ַ�����С���� 
    this->ntm.c_cc[VMIN] = 1;   // ��ȡ��һ���ַ��ĵȴ�ʱ�� 

    // ������뻺��
    int rtn = 0;
    rtn = ::tcflush(this->fd, TCIFLUSH);
    if (rtn != 0)
    {
        return false;
    }
    // ����������
    rtn = ::tcflush(this->fd, TCOFLUSH);
    if (rtn != 0)
    {
        return false;
    }

    // �����������봮��
    rtn = ::tcsetattr(this->fd, TCSANOW, &this->ntm);
    if (rtn != 0)
    {
        return false;
    }

    return true;
}

bool FoxStty::sendData(const char* data, int dataLen, int& sendLen)
{
    if (this->fd < 0)
    {
        return false;
    }

    // ��ͣ�ķ������ݣ�ֱ��������Ϊֹ
    int postion = 0;
    while (postion < dataLen)
    {
        int len = ::write(this->fd, &data[postion], dataLen - postion);
        if (len > 0)
        {
            postion += len;
        }
        else if (len <= 0)
        {
            break;
        }
    }

    sendLen = postion;
    return sendLen == dataLen;
}

bool FoxStty::recvData(unsigned char* data, int dataLen, long uTimeout, int& recvLen)
{
    recvLen = 0;

    if (this->fd < 0)
    {
        return false;
    }

    // �������
    ::memset(data, 0, dataLen);

    // ����select������Ҫ��fd_set
    fd_set readset = { 0 };
    FD_ZERO(&readset);
    FD_SET((unsigned int)this->fd, &readset);

    // ָ��select�����ȴ�ʱ��1000΢��
    timeval tv = { 0 };
    tv.tv_sec = uTimeout / (1000 * 1000);
    tv.tv_usec = uTimeout % (1000 * 1000);

    // select��readset���Ƿ������������ı�
    int maxfd = this->fd + 1;
    if (::select(maxfd, &readset, NULL, NULL, &tv) < 0)
    {
        return false;
    }

    // ��鷵�ؽ��
    int rtn = FD_ISSET(this->fd, &readset);
    if (rtn != 0)
    {
        recvLen = ::read(this->fd, data, dataLen);
    }

    return true;
}

bool FoxStty::clearFlush()
{
    if (this->fd < 0)
    {
        return false;
    }

    // ������뻺��
    int rtn = 0;
    rtn = ::tcflush(this->fd, TCIFLUSH);
    if (rtn != 0)
    {
        return false;
    }

    // ����������
    rtn = ::tcflush(this->fd, TCOFLUSH);
    if (rtn != 0)
    {
        return false;
    }

    return true;
}

void FoxStty::close()
{
    this->closeThread();

    this->handler->handleClosed(this->fd);

    if (this->fd > 0)
    {
        ::tcsetattr(this->fd, TCSADRAIN, &this->otm);
        ::close(this->fd);
        this->fd = -1;
    }
}

void FoxStty::recvFunc(STLThreadObject* threadObj)
{
    FoxStty* stty = (FoxStty*)threadObj;
    unsigned char* data = (unsigned char*)stty->data;
    int dataLen = sizeof(stty->data);
    FoxSttyHandler* handler = stty->handler;
    int fd = stty->fd;

    int recvLen = 0;

    while (true)
    {
        // ��ȡ���ݣ�SELECT���ȴ�100���루���ַ�ʽ��usleepռ��CPUҪ�ͣ��ֲ���������ݵ���ʱ���������ݶ�ʧ��
        if (!stty->recvData(data, dataLen, stty->getRcvTimeOut(), recvLen))
        {
            ::usleep(100 * 1000);
            continue;
        }

        // �����Ƿ�Ϊ0
        if (recvLen == 0)
        {
            // ֪ͨ����100�����ڣ�û���յ�����
            handler->handleNoRead(fd);
            continue;
        }

        // ֪ͨ���Ѿ��յ�һ��������
        handler->handleRead(fd, data, recvLen);
    }
}

bool FoxStty::bindHandler(FoxSttyHandler* handler)
{
    if (handler == nullptr)
    {
        return false;
    }

    delete this->handler;
    this->handler = handler;

    return true;
}

void FoxStty::setRcvTimeOut(long uTimeOut)
{
    lock_guard<mutex> guard(this->lock);
    this->uTimeOut = uTimeOut;
}

long FoxStty::getRcvTimeOut()
{
    lock_guard<mutex> guard(this->lock);
    return this->uTimeOut;
}
