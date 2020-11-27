#pragma once

#include <thread>
#include <vector>
#include <list>
#include <map>

#include "STLRunnable.h"
#include "STLSemaphore.h"

using namespace std;

/*
* 线程池：模仿JAVA的ExecutorService线程池模型，可以通过创建一个线程池，然后提交一堆的runnable给线程池中的线程执行。
*   通过线程池可以达到几个目的：
*   1、可以通过几个线程来并发执行大量的runnable任务，节省了操作系统宝贵的线程资源
*   2、避免让操作系统反复创建/删除线程，带来的可靠性问题和稳定性问题
* 使用步骤：
*   1、通过newThreadPool()创建一个线程池实例
*   2、在该线程池上，不断执行execute(new STLDemoRunnable())方法，提交一系列任务到线程池上执行
*   3、在各任务执行完毕后，对该线程池实力使用shutdownThreadPool()函数进行释放线程池
* 注意事项：
*   1、STLRunnable.run()函数不要长期挂起，否则期间会持续占用一个线程，导致别的STLRunnable任务得不到线程来执行，同时在shutdownThreadPool的时候
*      也被迫持续等待STLRunnable.run()的结束。
*   2、如果STLRunnable确实是需要长期挂起的，那么要针对该类型的STLRunnable去申请独享的该线程池，并且线程池shutdownThreadPool之前，STLRunnable有办法
*      全部取消挂起状态
*/
class STLThreadPool
{
public:
	STLThreadPool();
	virtual ~STLThreadPool();

public:
	// <1> 创建一个线程池：线程池的线程被创建起来，并等待信号
	static STLThreadPool* newThreadPool(int nThreads);

public:
	// <2> 提交一个runnable（runnable会被释放掉线程池在主动释放掉，所以要用new STLDemoRunnable()的方法传参）
	void    execute(STLRunnable* runnable);

public:
	// <3 >关闭线程池
	static void shutdownThreadPool(STLThreadPool* &threadPool);

public:
	bool isBusy();

private:
	// 内部线程函数
	static void executThreadFun(STLThreadPool* threadPoo);

private:// 线程结束状况
	void	setFinished();
	bool	getFinished();
	void	setRunning(bool increase);
	int		getRunning();
	void	setExit(bool isExit);
	bool	getExit();

	bool	isExit;    // 请求退出标识
	size_t	nFinished; // 退出的线程数
	size_t	nRunning; //  执行runnable的线程数

private:
	STLRunnable* getRunnable();

private:
	mutex				lock;// 互斥 
	list<thread*>		threads;// 线程数组	
	list<STLRunnable*>	runnables;// 等待执行的执行器集合
	STLSemaphore        semaphore;// 激活一个线程的信号灯	
};

