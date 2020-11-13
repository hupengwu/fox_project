#pragma once


#include <mutex>
#include <condition_variable>

/*
* 信号灯：
	A线程：发出一个信号，通知wait该信号的B线程某个实例可以进行下一步操作了；
	B线程：它的某个线程实例收到通知后，会取走一个信号。
  注意：信号灯通知的是某个线程实例
*/
class STLSemaphore
{
public:
	STLSemaphore(long count = 0);// 初始化计数值
	virtual ~STLSemaphore();

public:
	void signal();	// 发出一个信号（并通知一个线程）
	void wait();	// 等待一个信号

	void reset();	// 重置信号灯，使之可被重新使用

private:
	std::mutex				m_mutex;
	std::condition_variable m_cv;
	long					m_count;
};

