#pragma once

#include <pthread.h>
#include <iostream>
#include <time.h>
#include <errno.h>

#include "FoxTypeDef.h"
#include "FoxMacrDef.h"

/*
* 条件信号：对线程对象发出一个条件信号
*/
class  FoxPThreadCond
{
public:
	FoxPThreadCond()
	{
		pthread_mutex_init(&m_mu, NULL);
		pthread_cond_init(&m_cn, NULL);
		m_icount = 0;
	}
	virtual ~FoxPThreadCond()
	{
		pthread_mutex_destroy(&m_mu);
		pthread_cond_destroy(&m_cn);
	}

public:

	BOOL Wait() // O K
	{
		int retv = 0;

		if (pthread_mutex_lock(&m_mu) < 0)
		{
			return FALSE;
		}

		while (m_icount <= 0)
		{
			if ((retv = pthread_cond_wait(&m_cn, &m_mu)) < 0)
			{
				break;
			}
		}

		if (m_icount > 0 && retv == 0)
		{
			m_icount--;
		}

		pthread_mutex_unlock(&m_mu);

		if (retv != 0)
		{
			return FALSE;
		}

		return TRUE;
	}


	BOOL TimeWait(DWORD dwMilliseconds)
	{
		long ret = 0;
		timespec timeout;

		pthread_mutex_lock(&m_mu);

		timeout.tv_sec = getTime(NULL);
		timeout.tv_nsec = dwMilliseconds;

		if (m_icount <= 0)
		{
			ret = pthread_cond_timedwait(&m_cn, &m_mu, &timeout);
		}

		if (ret == 0)
		{
			m_icount--;
		}
		pthread_mutex_unlock(&m_mu);

		if (ret != 0)
		{
			return FALSE;
			//f(ret == ETIMEDOUT)
			//{
			//	return FALSE;
			//}
		}

		return TRUE;
	};



	BOOL Release()
	{
		int retv = 0;

		if (pthread_mutex_lock(&m_mu) < 0)
		{
			return FALSE;
		}

		retv = pthread_cond_signal(&m_cn);

		if (retv == 0)
		{
			m_icount++;
		}

		if (pthread_mutex_unlock(&m_mu) < 0)
		{
			return FALSE;
		}

		if (retv != 0)
		{
			return FALSE;
		}

		return TRUE;

	};

	BOOL Init()
	{
		m_icount = 0;
		if (pthread_mutex_init(&m_mu, NULL))
			return FALSE;
		if (pthread_cond_init(&m_cn, NULL))
			return FALSE;
		return TRUE;
	};
	BOOL Destroy()
	{
		int retv;
		retv = pthread_mutex_destroy(&m_mu);
		if (pthread_cond_destroy(&m_cn))
			return -1;
		return (retv ? FALSE : TRUE);
	};
	void p_operation_cleanup(void* arg)
	{
		pthread_mutex_t* sem;

		sem = (pthread_mutex_t*)arg;
		pthread_mutex_unlock(sem);
	}

private:
	//  隐藏拷贝构造函数
	FoxPThreadCond(const FoxPThreadCond& src);

	pthread_mutex_t  m_mu;
	pthread_cond_t  m_cn;
	int  m_icount;
};
