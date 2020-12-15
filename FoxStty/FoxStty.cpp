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
    // 生成串口设备的名称
    ::sprintf(this->name, "/dev/%s", name);

    // 打开并且设置串口
    int oflag = 0;
    oflag |= O_RDWR;// 读、写打开
    oflag |= O_NOCTTY;// 不将此设备分配作为此进程的控制终端
    oflag |= O_NONBLOCK;// 设置非阻塞方式
    this->fd = ::open(this->name, oflag);
    if (this->fd < 0)
    {
        return false;
    }

    // 取得并且保存原来的设置 
    int rtn = ::tcgetattr(this->fd, &this->otm);
    if (rtn != 0)
    {
        rtn = ::close(this->fd);
        this->fd = -1;
        return false;
    }

    // 通知打开串口
    this->handler->handleOpen(this->fd);

    // <6> 启动一个后台收发的线程
    this->createThread();

    return true;
}

bool FoxStty::isOpen()
{
    return this->fd > 0;
}

bool FoxStty::setParam(int speed, int databits, int stopbits, TTYParity parity)
{
    // 清空tm数据
    ::bzero(&this->ntm, sizeof(this->ntm));

    // 就是将终端设置为原始模式
    ::cfmakeraw(&this->ntm);

    // <1> 设置波特率
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

    // <2> 设置数据位
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

    // <3> 设置奇偶校验位数 
    switch (parity)
    {
    case N:
        this->ntm.c_cflag &= ~PARENB; /* Clear parity enable */
        this->ntm.c_iflag &= ~INPCK; /* Enable parity checking */
        break;
    case O:
        this->ntm.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
        this->ntm.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case E:
        this->ntm.c_cflag |= PARENB; /* Enable parity */
        this->ntm.c_cflag &= ~PARODD; /* 转换为偶效验*/
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

    // <4> 设置停止位   
    switch (stopbits)
    {
    case 1:
        this->ntm.c_cflag &= ~CSTOPB;// 1位停止
        break;
    case 2:
        this->ntm.c_cflag |= CSTOPB;// 2位停止位 
        break;
    default:
        this->ntm.c_cflag &= ~CSTOPB;
        break;
    }

    // 设置控制字符
    this->ntm.c_cc[VTIME] = 1;  // 读取字符的最小数量 
    this->ntm.c_cc[VMIN] = 1;   // 读取第一个字符的等待时间 

    // 清除输入缓存
    int rtn = 0;
    rtn = ::tcflush(this->fd, TCIFLUSH);
    if (rtn != 0)
    {
        return false;
    }
    // 清除输出缓存
    rtn = ::tcflush(this->fd, TCOFLUSH);
    if (rtn != 0)
    {
        return false;
    }

    // 将属性设置入串口
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

    // 不停的发送数据，直到发送完为止
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

    // 清空数据
    ::memset(data, 0, dataLen);

    // 设置select串口需要的fd_set
    fd_set readset = { 0 };
    FD_ZERO(&readset);
    FD_SET((unsigned int)this->fd, &readset);

    // 指明select的最大等待时间1000微秒
    timeval tv = { 0 };
    tv.tv_sec = uTimeout / (1000 * 1000);
    tv.tv_usec = uTimeout % (1000 * 1000);

    // select：readset中是否有描述符被改变
    int maxfd = this->fd + 1;
    if (::select(maxfd, &readset, NULL, NULL, &tv) < 0)
    {
        return false;
    }

    // 检查返回结果
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

    // 清除输入缓存
    int rtn = 0;
    rtn = ::tcflush(this->fd, TCIFLUSH);
    if (rtn != 0)
    {
        return false;
    }

    // 清除输出缓存
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
        // 读取数据：SELECT最大等待100毫秒（这种方式比usleep占用CPU要低，又不会出现数据到达时缓冲区数据丢失）
        if (!stty->recvData(data, dataLen, stty->getRcvTimeOut(), recvLen))
        {
            ::usleep(100 * 1000);
            continue;
        }

        // 长度是否为0
        if (recvLen == 0)
        {
            // 通知：在100毫秒内，没有收到数据
            handler->handleNoRead(fd);
            continue;
        }

        // 通知：已经收到一部分数据
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
