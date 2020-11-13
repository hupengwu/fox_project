#pragma once


#include <mutex>
#include <condition_variable>

/*
* �źŵƣ�
	A�̣߳�����һ���źţ�֪ͨwait���źŵ�B�߳�ĳ��ʵ�����Խ�����һ�������ˣ�
	B�̣߳�����ĳ���߳�ʵ���յ�֪ͨ�󣬻�ȡ��һ���źš�
  ע�⣺�źŵ�֪ͨ����ĳ���߳�ʵ��
*/
class STLSemaphore
{
public:
	STLSemaphore(long count = 0);// ��ʼ������ֵ
	virtual ~STLSemaphore();

public:
	void signal();	// ����һ���źţ���֪ͨһ���̣߳�
	void wait();	// �ȴ�һ���ź�

	void reset();	// �����źŵƣ�ʹ֮�ɱ�����ʹ��

private:
	std::mutex				m_mutex;
	std::condition_variable m_cv;
	long					m_count;
};

