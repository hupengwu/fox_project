#include "FoxPThreadModule.h"


FoxPThreadModule::FoxPThreadModule() :m_tid(0), m_pfunThreadProc(NULL), m_bWorking(FALSE), m_bPausing(FALSE)
{

}
FoxPThreadModule::~FoxPThreadModule()
{
}


BOOL FoxPThreadModule::Create()
{
	// 安全性检查
	if (m_bWorking)
	{
		return TRUE;
	}

	// 安全性检查
	if (m_pfunThreadProc == NULL)
	{
		return FALSE;
	}

	

	int ret = 0;
	try
	{
		// 创建一个缺省的属性对象
		ret = pthread_attr_init(&m_tattr);


		// 创建新的线程,arg用于存放要传递给所调用函数的参数
		ret = pthread_create(&m_tid, &m_tattr, m_pfunThreadProc, (void*)this);
	}
	catch (...)
	{
		return FALSE;
	}

	if (ret != 0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL FoxPThreadModule::Start(int nPriority)
{
	if (!Create())
	{
		return FALSE;
	}
	m_bWorking = TRUE;

	m_StartCond.Release();

	return TRUE;
}


BOOL FoxPThreadModule::Exit()
{
	// 检查线程是否启动
	if (!m_bWorking && !m_bPausing)
	{
		return TRUE;
	}

	// 强制退出
	if (pthread_cancel(GetThreadID()) == 0)
	{
		m_bWorking = FALSE;
		m_bPausing = FALSE;
#ifdef _DEBUG		
		_Debug_Thread_Print("thread Cancel");
#endif
		// 	std::cout << "sign cancel" << std::endl;

		return TRUE;
	}

	return FALSE;
}

BOOL FoxPThreadModule::Stop()
{
	// 检查线程是否已经运行
	if (!m_bWorking)
	{
		return FALSE;
	}

	m_StopCond.Release();

	return TRUE;
}

BOOL FoxPThreadModule::Resume()
{
	// 检查线程是否已经运行
	if (!m_bWorking)
	{
		return FALSE;
	}

	m_ResumeCond.Release();

	return TRUE;
}

BOOL FoxPThreadModule::Pause()
{
	// 检查线程是否已经运行
	if (!m_bWorking)
	{
		return FALSE;
	}

	m_PausepCond.Release();

	return TRUE;
}

BOOL FoxPThreadModule::IsWorking()
{
	return m_bWorking;
}

BOOL FoxPThreadModule::IsPausing()
{
	return m_bPausing;
}

// 函数说明:设置线程函数的函数体
// 输入参数:pfunThreadProc=线程函数的函数指针
// 返 回 值:操作是否成功
BOOL FoxPThreadModule::SetThreadProc(funThreadProc pfunThreadProc)
{
	if (pfunThreadProc == NULL)
	{
		return FALSE;
	}
	if (m_pfunThreadProc != NULL)
	{
		return FALSE;
	}

	m_pfunThreadProc = pfunThreadProc;
	return TRUE;
}

void FoxPThreadModule::Sleep(DWORD dwMilliseconds)
{
	DWORD useconds = dwMilliseconds * 1000;
	usleep(useconds);
}

// 函数说明:等待事件
// 输入参数:dwSleep=等待事件的持续时间
//			dwSpace-循环等待事件的间隔
// 返 回 值:-1:退出事件;0=忽略事件
// 备    注:可以对退出/暂停/继续事件进行
DWORD FoxPThreadModule::WaitForEvent(DWORD dwSleep, DWORD dwSpace)
{
	m_bPausing = FALSE;

	// 输入参数的合法性检查
	if (dwSleep < 1)
	{
		dwSleep = 1;
	}
	if (dwSpace < 1)
	{
		dwSpace = 1;
	}

	for (DWORD i = 0; i < dwSleep; i += dwSpace)
	{
		do
		{
			// 延时:防止暂停大量的占用CPU时间
			if (m_bPausing)
			{
				Sleep(dwSleep);
			}

			if ((!m_bPausing) && (dwSpace != 0))
			{
				Sleep(dwSpace);
			}

			// 等待退出事件(0=期间并不阻塞)
		//	if (::WaitForSingleObject(m_StopEvent.m_hObject,0) == WAIT_OBJECT_0)
			//if(m_bStopEvent)
			//if(m_StopCond.Wait())
			if (m_StopCond.TimeWait(0))
			{
				// 登记线程状态
				m_bWorking = FALSE;
				m_bPausing = FALSE;
#ifdef _DEBUG
				Debug_Thread_Print("thread stop");
#endif
				// 	std::cout<< "signal  Thread stop" << std::endl;

				return ((DWORD)-1);
			}

			// 当继续线程事件到来时,将暂停标记取消
			//if (::WaitForSingleObject(m_ResumeEvent.m_hObject,0) == WAIT_OBJECT_0)
			if (m_ResumeCond.TimeWait(0))
			{
				m_bPausing = FALSE;
#ifdef _DEBUG
				Debug_Thread_Print("signal  thread Resume");
#endif
				// 	std::cout<< "signal  Thread Resume" << std::endl;
			}

			// 当有暂停线程私交到来时,将暂停线程取消
			//if (::WaitForSingleObject(m_PauseEvent.m_hObject,0) == WAIT_OBJECT_0)
			if (m_PausepCond.TimeWait(0))
			{
				m_bPausing = TRUE;
#ifdef _DEBUG
				Debug_Thread_Print("signal  thread Pause");
#endif
				//   std::cout<< " signal Thread Pause" << std::endl;
				continue;
			}

		} while (m_bPausing);
	}

	return 0;
}

// 函数说明:等待事件
// 输入参数:dwSleep=等待事件的持续时间
//			dwSpace-循环等待事件的间隔
// 返 回 值:-1:退出事件;0=忽略事件
// 备    注:可以对退出/暂停/继续事件进行
BOOL FoxPThreadModule::WaitForStart()
{
	// 	::WaitForSingleObject(m_StartEvent.m_hObject,INFINITE);	
	m_StartCond.Wait();

	// 登记线程状态
	m_bWorking = TRUE;
#ifdef _DEBUG
	Debug_Thread_Print("thread Start");
#endif

	return TRUE;
}

// ----------------------------------------------------------------------
// 功能及调用后状态：会阻塞调用它的线程，直到本线程结束
//                   本线程结束，本线程的资源被收回
// 备注：
// 不能有多个线程等待同一个线程终止，(否则pthread_join()返回错误码 ESRCH)
// ----------------------------------------------------------------------
BOOL FoxPThreadModule::Join()
{
	if (!IsWorking())
	{
		return FALSE;
	}

	void* status; // 接收指定线程终止时的返回状态码

// 	返回值：成功返回 0
// 	ESRCH  tid 指定的线程不是一个当前线程中合法且未分离的线程。
// 	EDEADLK  tid 指定的是当前线程。	
// 	EINVAL  tid 非法。
	int ret = pthread_join(GetThreadID(), &status);

	if (ret != 0)
	{
		return FALSE;
	}

	return TRUE;
}

void FoxPThreadModule::_Debug_Thread_Print(char* exp)
{
#ifdef _DEBUG
	char FileName[] = "Debug_Thread.txt";

	std::ofstream fout(FileName, std::ios::app);

	fout << exp << ",TID:" << GetThreadID() << std::endl;

	fout.close();

#endif
};