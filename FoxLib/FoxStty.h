#pragma once
#include <pthread.h> 
#include <termios.h> 

// �����豸��Ϣ�ṹ 
typedef struct tty_info_t
{
    int fd; // �����豸ID 
    pthread_mutex_t mt; // �߳�ͬ��������� 
    char name[24]; // �����豸���ƣ�����"/dev/ttyS0" 
    struct termios ntm; // �µĴ����豸ѡ�� 
    struct termios otm; // �ɵĴ����豸ѡ�� 
} TTY_INFO;

/*��ͷ�ļ��еĺ������岻�ѿ����������Ĺ��ܣ�ʹ�ù������£�
��1�� �򿪴����豸�����ú���setTTYSpeed������
��2�� ���ô��ڶ�д�Ĳ����ʣ����ú���setTTYSpeed������
��3�� ���ô��ڵ����ԣ�����ֹͣλ��У��λ������λ�ȣ����ú���setTTYParity������
��4�� �򴮿�д�����ݣ����ú���sendnTTY������
��5�� �Ӵ��ڶ������ݣ����ú���recvnTTY������
��6�� ������ɺ���Ҫ���ú���cleanTTY�������ͷ�����Ĵ�����Ϣ�ӿڣ�
���У�lockTTY������unlockTTY������Ϊ���ܹ��ڶ��߳���ʹ�á��ڶ�д������ǰ����Ҫ�������ͷŴ�����Դ��
�����ʹ�÷������ڴ���ʵ�ֵ�ԭ�ļ��У�main���������н�������ʾ���������Դ�����ļ��� */

enum TTYParity { N, E, O, S };

class FoxStty
{
public:
    // ��ʼ�������豸������ԭ�����õı��� 
    static TTY_INFO* readyTTY(int id);

    // ���ô���ͨ������ 
    static int setTTYSpeed(TTY_INFO* ptty, int speed);

    // ���ô�������λ��ֹͣλ��Ч��λ
    static int setTTYParity(TTY_INFO* ptty, int databits, TTYParity parity, int stopbits);

    // �������豸��Դ 
    static int cleanTTY(TTY_INFO* ptty);

    // ���ʹ�������
    static int sendnTTY(TTY_INFO* ptty, char* pbuf, int size);

    // ��ȡ��������
    static int recvnTTY(TTY_INFO* ptty, char* pbuf, int size);

public:
    // ��������
    static int lockTTY(TTY_INFO* ptty);

    // ��������
    static int unlockTTY(TTY_INFO* ptty);

public:
    // ��������
    static int demo();
};

