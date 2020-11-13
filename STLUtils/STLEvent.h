#pragma once

#include <mutex>
#include <condition_variable>

using namespace std;

class STLEvent
{
public:	
	void signal();// 发出事件:通知一个线程	
	void wait();// 等待事件

	// 等待事件：最多等待指定的时间，是否为超时返回
	cv_status waitFor(int ms);//等待事件

public:
	void reset();// 重置事件

private:
	mutex m_mutex;// 互斥
	condition_variable m_cv;// 条件变量
	bool m_waited = true;// 是否需要wait
};

