#pragma once

#include "STLThreadPool.h"
#include <thread>

using namespace std;

class STLTimedTask
{
public:
	STLTimedTask(STLRunnable* runnable,bool fixedInterval = true);
	virtual ~STLTimedTask();

public:
	// ����һ����ʱ��
	void start(int timeInterval);

	// �رն�ʱ��
	void close();

	// �޸�ʱ����
	void setInterval(int timeInterval);

private:// �߳̽���״��
	void	setFinished();
	bool	getFinished();
	void	setExit(bool isExit);
	bool	getExit();

	bool	isExit;    // �����˳���ʶ
	bool	bFinished; // �˳����߳���

private:
	// �ڲ��̺߳���
	static void executThreadFun(STLTimedTask& threadPoo);

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



