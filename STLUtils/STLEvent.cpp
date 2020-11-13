#include "STLEvent.h"

void STLEvent::signal()
{
	unique_lock<mutex> lock(m_mutex);

	// ��ʶ������Ҫ�ȴ���
	this->m_waited = false;

	// ֪ͨһ���̼߳���Wait()���ʶ
	m_cv.notify_one();
}

void STLEvent::wait()
{
	unique_lock<mutex> lock(m_mutex);

	// �ȴ�m_waited==false����������
	m_cv.wait(lock, [=] {return this->m_waited==false; });
}

cv_status STLEvent::waitFor(int ms)
{
	unique_lock<mutex> lock(m_mutex);

	// �ȴ�m_waited==false����������
	return m_cv.wait_for(lock, chrono::microseconds(ms));
}


void STLEvent::reset()
{
	unique_lock<mutex> lock(m_mutex);

	// ��ʶ�������˵ȴ��ı�ʶ
	this->m_waited = true;

	m_cv.notify_one();
}
