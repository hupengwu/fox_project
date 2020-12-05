#pragma once

#include "STLThreadPool.h"
#include <thread>

using namespace std;

/*
* 定时器：内置一个线程的轻量级定时器，可以定时执行某个动作。
*   通过定时器可以达到几个目的：
*   1、异步线程的思想是过程化思想，整个过程相对比较难以理解，而定时器是不断被激活的异步动作，相对比较简单
* 使用步骤：
*   1、通过STLTimedTask getTime(new STLDemoRunnable())构造一个定时器对象
*   2、启动定时器time.start()，此时线程就被启动，STLDemoRunnable的动作就不断被定时执行
*   3、程序退出时，需要关闭定时器的时候，time.close()
* 注意事项：
*   1、STLRunnable.run()函数不要长期挂起，否则STLTimedTask会被迫持续等待STLRunnable.run()的结束。
*   2、如果STLRunnable确实是需要长期挂起的，那么STLTimedTask.close()之前，STLRunnable有办法全部取消挂起状态
*/
class STLTimedTask
{
public:
	// <1> 定时器实例化
	STLTimedTask(STLRunnable* runnable,bool fixedInterval = true);
	virtual ~STLTimedTask();

public:
	// <2> 启动定时器
	void create(int timeInterval);

	// <3> 关闭定时器
	void close();

	// 修改时间间隔
	void setInterval(int timeInterval);

private:// 线程结束状况
	void				setFinished();
	bool				getFinished();
	void				setExit(bool isExit);
	bool				getExit();

	bool				isExit;    // 请求退出标识
	bool				bFinished; // 是否已经退出线程

private:
	// 内部线程函数
	static void			executThreadFun(STLTimedTask& timedTask);

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



