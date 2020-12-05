#pragma once

#include <termios.h> 
#include <linux/serial.h> 
#include <fcntl.h>
#include <unistd.h> 
#include  <string.h>

#include <STLThreadObject.h>
#include "FoxSttyHandler.h"

namespace fox
{
	/*
	* 校验位
	*/
	enum TTYParity { N, E, O, S };
}

using namespace fox;
using namespace std;

/*
* 串口封装类型：
* https://blog.csdn.net/onion_lwl/article/details/81293266
* https://blog.csdn.net/sno_guo/article/details/17799739?utm_source=blogkpcl2
* https://blog.csdn.net/developerof/article/details/82317540
*/
class FoxStty : public STLThreadObject
{
public:
	FoxStty();
	virtual ~FoxStty();

public:
	/*
	* 打开串口：dev下的ttyS0、ttyS1等串口设备名称
	*/
	bool open(const char* name = "ttyS0");	

	/*
	* 设置通信参数:速率/数据位/停止位/校验位
	*/
	bool setParam(int speed = 9600, int databits = 8, int stopbits = 1, TTYParity parity = N);

	/*
	* 写数据：待发送数据的长度和实际发送数据的长度
	*/
	bool writeData(const char* data, int dataLen, int& sendLen);

	/*
	* 读数据
	*/
	bool readData(unsigned char* data, int dataLen, int& recvLen);

	/*
	* 关闭串口
	*/
	void close();


	/*
	* 自定义接收数据处理者FoxSttyHandler，它会被自动释放
	*/
	bool bindHandler(FoxSttyHandler* handler);

private:
	/*
	* 子类需要实现的接受数据处理函数
	*/
	virtual void recvFunc(STLThreadObject* threadObj);

	/*
	* 接收/关闭/打开的处理人
	*/
	FoxSttyHandler*		handler;

private:
	int         fd;        // 串口设备ID     
	char        name[24];  // 串口设备名称，例："/dev/ttyS0" 
	termios     ntm;       // 新的串口设备选项 
	termios     otm;       // 旧的串口设备选项
};

