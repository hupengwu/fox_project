#pragma once

#include <mutex>
#include <condition_variable>

using namespace std;

class STLEvent
{
public:	
	void signal();// �����¼�:֪ͨһ���߳�	
	void wait();// �ȴ��¼�

	// �ȴ��¼������ȴ�ָ����ʱ�䣬�Ƿ�Ϊ��ʱ����
	cv_status waitFor(int ms);//�ȴ��¼�

public:
	void reset();// �����¼�

private:
	mutex m_mutex;// ����
	condition_variable m_cv;// ��������
	bool m_waited = true;// �Ƿ���Ҫwait
};

