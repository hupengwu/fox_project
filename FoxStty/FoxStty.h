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
	* У��λ
	*/
	enum TTYParity { N, E, O, S };
}

using namespace fox;
using namespace std;

/*
* ���ڷ�װ���ͣ����ڼ򵥶��̣߳�ʵ���˴��ڵ��첽�շ�����
*   1�������������ݵ�ʱ�򣬿���ͨ��FoxStty�����sendData()�������ݷ���
*   2���������ݣ�����ͨ��������FoxSttyHandler�����࣬�������ݵı�����Ӧ����
* �ο���
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
	* �򿪴��ڣ�dev�µ�ttyS0��ttyS1�ȴ����豸����
	*/
	bool open(const char* name = "ttyS0");	

	/*
	* ����ͨ�Ų���:����/����λ/ֹͣλ/У��λ
	*/
	bool setParam(int speed = 9600, int databits = 8, int stopbits = 1, TTYParity parity = N);

	/*
	* д���ݣ����������ݵĳ��Ⱥ�ʵ�ʷ������ݵĳ���
	*/
	bool sendData(const char* data, int dataLen, int& sendLen);

	/*
	* ��ջ���
	*/
	bool clearFlush();

	/*
	* �رմ���
	*/
	void close();

	/*
	* �Զ���������ݴ�����FoxSttyHandler�����ᱻ�Զ��ͷ�
	*/
	bool bindHandler(FoxSttyHandler* handler);

	/*
	* ������ѯ�̵߳�SELECT��ʱ�������ʱ��Χû�����ݽ��յ���handler��handleNoRead()һ��
	*/
	void setTimeOut(long uTimeOut);

private:
	/*
	* ������
	*/
	bool recvData(unsigned char* data, int dataLen, long uTimeout, int& recvLen);

private:
	/*
	* ������Ҫʵ�ֵĽ������ݴ�����
	*/
	virtual void		recvFunc(STLThreadObject* threadObj);

	/*
	* ����/�ر�/�򿪵Ĵ�����
	*/
	FoxSttyHandler*		handler;

private:
	int         fd;        // �����豸ID     
	char        name[24];  // �����豸���ƣ�����"/dev/ttyS0" 
	termios     ntm;       // �µĴ����豸ѡ�� 
	termios     otm;       // �ɵĴ����豸ѡ��
	char		data[1024];// ���յ�����
	long        uTimeOut;  // SELECT��ʱ
};

