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
	// 创建一个定时器
	void start(int timeInterval);

	// 关闭定时器
	void close();

	// 修改时间间隔
	void setInterval(int timeInterval);

private:// 线程结束状况
	void	setFinished();
	bool	getFinished();
	void	setExit(bool isExit);
	bool	getExit();

	bool	isExit;    // 请求退出标识
	bool	bFinished; // 退出的线程数

private:
	// 内部线程函数
	static void executThreadFun(STLTimedTask& threadPoo);

private:
	void				signal();
	void				wait();
	void				wait_util(chrono::system_clock::time_point nextPoint);
	condition_variable	cv;
	int					timeInterval;

private:
	mutex				lock;// 互斥 
	thread*				threadPtr;// 线程
	STLRunnable*        runnable;// 等待执行的执行器集合
	bool                fixedInterval;// 固定时间模式/间隔时间模式
	
};



