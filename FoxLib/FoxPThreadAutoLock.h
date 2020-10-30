#pragma once

#include "FoxPThreadMutex.h"

class FoxPThreadAutoLock
{
public:
	FoxPThreadAutoLock(FoxPThreadMutex& ctrlMutex)
	{
		m_mutex = &ctrlMutex;
		m_mutex->Lock();
	}

	virtual ~FoxPThreadAutoLock()
	{
		m_mutex->Unlock();
	}

private:
	FoxPThreadMutex* m_mutex;
};

