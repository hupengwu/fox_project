#pragma once

#include <pthread.h>

/*
*
<pthread.h>�ڹ��̵ı������Ӳ�����Ҫ����-lpthread����
*/

class FoxPThreadMutex
{
public:
	FoxPThreadMutex()
	{
		pthread_mutex_init(&m_cs, NULL);
	}
	virtual ~FoxPThreadMutex()
	{
		pthread_mutex_destroy(&m_cs);
	}

public:
	void Lock()
	{
		pthread_mutex_lock(&m_cs);
	}
	void Unlock()
	{
		pthread_mutex_unlock(&m_cs);
	}

private:
	pthread_mutex_t m_cs;
};

