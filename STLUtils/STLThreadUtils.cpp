#include "STLThreadUtils.h"

void fun1(int n)  //��ʼ�����캯��  
{
    n += 10;
    this_thread::sleep_for(chrono::milliseconds(10));
}
void fun2(int& n) //�������캯��  
{
    n += 20;
    this_thread::sleep_for(chrono::milliseconds(10));
}

int STLThreadUtils::demo()
{
    int n = 0;

    // �̶߳���1��ͨ������1�ͺ�����Ҫ�ġ�ֵ������������һ���̶߳���
    thread t1(fun1, n + 1);  //����ֵ����  ��n�����ڸ����̶߳���һ������
    t1.join();// �ȴ��߳̽���


    // �̶߳���2��ͨ������2�ͺ�����Ҫ�ġ����á�����������һ���̶߳���
    n = 10;
    thread t2(fun2, ref(n)); //����  ��n�����������̹߳�ͬ�Ŀ��̲߳��������Ҫע��ͨ���ٽ������б���
    //û��join�������߳̿��ܻ�û����

    //  �̶߳���3�����̶߳���2ִ������
    thread t3(move(t2));     //t3ִ��t2��t2����thread  
    t3.join();//�ȴ��߳̽���

    return 0;
}
