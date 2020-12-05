#pragma once

#include "STLThreadPool.h"
#include <thread>

using namespace std;

/*
* ��ʱ��������һ���̵߳���������ʱ�������Զ�ʱִ��ĳ��������
*   ͨ����ʱ�����Դﵽ����Ŀ�ģ�
*   1���첽�̵߳�˼���ǹ��̻�˼�룬����������ԱȽ�������⣬����ʱ���ǲ��ϱ�������첽��������ԱȽϼ�
* ʹ�ò��裺
*   1��ͨ��STLTimedTask getTime(new STLDemoRunnable())����һ����ʱ������
*   2��������ʱ��time.start()����ʱ�߳̾ͱ�������STLDemoRunnable�Ķ����Ͳ��ϱ���ʱִ��
*   3�������˳�ʱ����Ҫ�رն�ʱ����ʱ��time.close()
* ע�����
*   1��STLRunnable.run()������Ҫ���ڹ��𣬷���STLTimedTask�ᱻ�ȳ����ȴ�STLRunnable.run()�Ľ�����
*   2�����STLRunnableȷʵ����Ҫ���ڹ���ģ���ôSTLTimedTask.close()֮ǰ��STLRunnable�а취ȫ��ȡ������״̬
*/
class STLTimedTask
{
public:
	// <1> ��ʱ��ʵ����
	STLTimedTask(STLRunnable* runnable,bool fixedInterval = true);
	virtual ~STLTimedTask();

public:
	// <2> ������ʱ��
	void create(int timeInterval);

	// <3> �رն�ʱ��
	void close();

	// �޸�ʱ����
	void setInterval(int timeInterval);

private:// �߳̽���״��
	void				setFinished();
	bool				getFinished();
	void				setExit(bool isExit);
	bool				getExit();

	bool				isExit;    // �����˳���ʶ
	bool				bFinished; // �Ƿ��Ѿ��˳��߳�

private:
	// �ڲ��̺߳���
	static void			executThreadFun(STLTimedTask& timedTask);

private:
	void				signal();
	void				wait();
	void				wait_util(chrono::system_clock::time_point nextPoint);
	condition_variable	cv;
	int					timeInterval;

private:
	mutex				lock;// ���� 
	thread*				threadPtr;// �߳�
	STLRunnable*        runnable;// �ȴ�ִ�е�ִ��������
	bool                fixedInterval;// �̶�ʱ��ģʽ/���ʱ��ģʽ
	
};



