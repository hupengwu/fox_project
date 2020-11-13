#include "STLEvent.h"

void STLEvent::signal()
{
	unique_lock<mutex> lock(m_mutex);

	// 标识：不需要等待了
	this->m_waited = false;

	// 通知一个线程检在Wait()查标识
	m_cv.notify_one();
}

void STLEvent::wait()
{
	unique_lock<mutex> lock(m_mutex);

	// 等待m_waited==false的条件到来
	m_cv.wait(lock, [=] {return this->m_waited==false; });
}

cv_status STLEvent::waitFor(int ms)
{
	unique_lock<mutex> lock(m_mutex);

	// 等待m_waited==false的条件到来
	return m_cv.wait_for(lock, chrono::microseconds(ms));
}


void STLEvent::reset()
{
	unique_lock<mutex> lock(m_mutex);

	// 标识：重置了等待的标识
	this->m_waited = true;

	m_cv.notify_one();
}
