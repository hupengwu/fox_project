#include "STLSemaphore.h"

STLSemaphore::STLSemaphore(long count) :m_count(count)
{
}

STLSemaphore::~STLSemaphore()
{
}

void STLSemaphore::signal()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_count++;
	m_cv.notify_one();
}

void STLSemaphore::wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	// [=] {return m_count > 0; }是lambda表达式，表示一个条件为m_count>0的匿名函数
	m_cv.wait(lock, [=] {return m_count > 0; });

	m_count--;
}

void STLSemaphore::reset()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_count = 0;
}
