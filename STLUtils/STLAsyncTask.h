#pragma once

#include "STLThreadPool.h"

class STLAsyncTask
{
public:
	STLAsyncTask();
	virtual ~STLAsyncTask();

public:
	// <1> ����һ���̳߳أ��̳߳ص��̱߳��������������ȴ��ź�
	void create(int nThreads);

public:
	// <2> �ύһ��runnable��runnable�ᱻ�ͷŵ��̳߳��������ͷŵ�������Ҫ��new STLDemoRunnable()�ķ������Σ�
	void    execute(STLRunnable* runnable);

public:
	// <3 >�ر��̳߳�
	void close();

public:
	bool isBusy();

private:
	// �̳߳�
	STLThreadPool* threadPool;
};

