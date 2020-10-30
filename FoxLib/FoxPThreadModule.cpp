#include "FoxPThreadModule.h"


FoxPThreadModule::FoxPThreadModule() :m_tid(0), m_pfunThreadProc(NULL), m_bWorking(FALSE), m_bPausing(FALSE)
{

}
FoxPThreadModule::~FoxPThreadModule()
{
}


BOOL FoxPThreadModule::Create()
{
	// ��ȫ�Լ��
	if (m_bWorking)
	{
		return TRUE;
	}

	// ��ȫ�Լ��
	if (m_pfunThreadProc == NULL)
	{
		return FALSE;
	}

	

	int ret = 0;
	try
	{
		// ����һ��ȱʡ�����Զ���
		ret = pthread_attr_init(&m_tattr);


		// �����µ��߳�,arg���ڴ��Ҫ���ݸ������ú����Ĳ���
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
	// ����߳��Ƿ�����
	if (!m_bWorking && !m_bPausing)
	{
		return TRUE;
	}

	// ǿ���˳�
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
	// ����߳��Ƿ��Ѿ�����
	if (!m_bWorking)
	{
		return FALSE;
	}

	m_StopCond.Release();

	return TRUE;
}

BOOL FoxPThreadModule::Resume()
{
	// ����߳��Ƿ��Ѿ�����
	if (!m_bWorking)
	{
		return FALSE;
	}

	m_ResumeCond.Release();

	return TRUE;
}

BOOL FoxPThreadModule::Pause()
{
	// ����߳��Ƿ��Ѿ�����
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

// ����˵��:�����̺߳����ĺ�����
// �������:pfunThreadProc=�̺߳����ĺ���ָ��
// �� �� ֵ:�����Ƿ�ɹ�
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

// ����˵��:�ȴ��¼�
// �������:dwSleep=�ȴ��¼��ĳ���ʱ��
//			dwSpace-ѭ���ȴ��¼��ļ��
// �� �� ֵ:-1:�˳��¼�;0=�����¼�
// ��    ע:���Զ��˳�/��ͣ/�����¼�����
DWORD FoxPThreadModule::WaitForEvent(DWORD dwSleep, DWORD dwSpace)
{
	m_bPausing = FALSE;

	// ��������ĺϷ��Լ��
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
			// ��ʱ:��ֹ��ͣ������ռ��CPUʱ��
			if (m_bPausing)
			{
				Sleep(dwSleep);
			}

			if ((!m_bPausing) && (dwSpace != 0))
			{
				Sleep(dwSpace);
			}

			// �ȴ��˳��¼�(0=�ڼ䲢������)
		//	if (::WaitForSingleObject(m_StopEvent.m_hObject,0) == WAIT_OBJECT_0)
			//if(m_bStopEvent)
			//if(m_StopCond.Wait())
			if (m_StopCond.TimeWait(0))
			{
				// �Ǽ��߳�״̬
				m_bWorking = FALSE;
				m_bPausing = FALSE;
#ifdef _DEBUG
				Debug_Thread_Print("thread stop");
#endif
				// 	std::cout<< "signal  Thread stop" << std::endl;

				return ((DWORD)-1);
			}

			// �������߳��¼�����ʱ,����ͣ���ȡ��
			//if (::WaitForSingleObject(m_ResumeEvent.m_hObject,0) == WAIT_OBJECT_0)
			if (m_ResumeCond.TimeWait(0))
			{
				m_bPausing = FALSE;
#ifdef _DEBUG
				Debug_Thread_Print("signal  thread Resume");
#endif
				// 	std::cout<< "signal  Thread Resume" << std::endl;
			}

			// ������ͣ�߳�˽������ʱ,����ͣ�߳�ȡ��
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

// ����˵��:�ȴ��¼�
// �������:dwSleep=�ȴ��¼��ĳ���ʱ��
//			dwSpace-ѭ���ȴ��¼��ļ��
// �� �� ֵ:-1:�˳��¼�;0=�����¼�
// ��    ע:���Զ��˳�/��ͣ/�����¼�����
BOOL FoxPThreadModule::WaitForStart()
{
	// 	::WaitForSingleObject(m_StartEvent.m_hObject,INFINITE);	
	m_StartCond.Wait();

	// �Ǽ��߳�״̬
	m_bWorking = TRUE;
#ifdef _DEBUG
	Debug_Thread_Print("thread Start");
#endif

	return TRUE;
}

// ----------------------------------------------------------------------
// ���ܼ����ú�״̬�����������������̣߳�ֱ�����߳̽���
//                   ���߳̽��������̵߳���Դ���ջ�
// ��ע��
// �����ж���̵߳ȴ�ͬһ���߳���ֹ��(����pthread_join()���ش����� ESRCH)
// ----------------------------------------------------------------------
BOOL FoxPThreadModule::Join()
{
	if (!IsWorking())
	{
		return FALSE;
	}

	void* status; // ����ָ���߳���ֹʱ�ķ���״̬��

// 	����ֵ���ɹ����� 0
// 	ESRCH  tid ָ�����̲߳���һ����ǰ�߳��кϷ���δ������̡߳�
// 	EDEADLK  tid ָ�����ǵ�ǰ�̡߳�	
// 	EINVAL  tid �Ƿ���
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