#pragma once

#include "STLThreadPool.h"

class STLAsyncTask
{
public:
	STLAsyncTask();
	virtual ~STLAsyncTask();

public:
	// <1> 创建一个线程池：线程池的线程被创建起来，并等待信号
	void create(int nThreads);

public:
	// <2> 提交一个runnable（runnable会被释放掉线程池在主动释放掉，所以要用new STLDemoRunnable()的方法传参）
	void    execute(STLRunnable* runnable);

public:
	// <3 >关闭线程池
	void close();

public:
	bool isBusy();

private:
	// 线程池
	STLThreadPool* threadPool;
};

