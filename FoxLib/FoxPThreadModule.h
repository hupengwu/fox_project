#pragma once



#include "FoxTypeDef.h"
#include "FoxPThreadCond.h"
//#include "../../FunctionLinux/Source/FunctionLinux.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <iostream>  
#include <fstream>


typedef void* (*funThreadProc) (void*);

// ���Ժ�
#ifdef _DEBUG
#define Debug_Thread_Print(exp) _Debug_Thread_Print(exp);
#define Debug_Thread_Print_Other(exp) pObject->_Debug_Thread_Print(exp);
#else
#define Debug_Thread_Print(exp) ((void)0)
#define Debug_Thread_Print_Other(exp) ((void)0)
#endif

// �궨��
//#define WAITFOREVENT_SLEEP_SPACE(dwSleep,dwSpace) if(pObject->WaitForEvent(dwSleep,dwSpace)==-1){return -1;}
#define WAITFOREVENT_SLEEP_SPACE(dwSleep,dwSpace) if(pObject->WaitForEvent(dwSleep,dwSpace)==((DWORD)-1)){return 0;}
#define WAITFOREVENT_SLEEP(dwSleep) WAITFOREVENT_SLEEP_SPACE(dwSleep,10)
#define WAITFOREVENT() WAITFOREVENT_SLEEP(10)

#define WAITFORSTART() pObject->WaitForStart();

#define THREAD_PRIORITY_NORMAL  20

class FoxPThreadModule
{
public:
	FoxPThreadModule();
	virtual ~FoxPThreadModule();

public:
	void _Debug_Thread_Print(char* exp);

	BOOL Exit();
	BOOL Stop();
	BOOL Start(int nPriority = THREAD_PRIORITY_NORMAL);
	BOOL Resume();
	BOOL Pause();
	BOOL IsWorking();
	BOOL IsPausing();

	BOOL Join();

	static void	Sleep(DWORD dwMilliseconds);

	pthread_t GetThreadID() const
	{
		return m_tid;
	};
protected:
	BOOL	SetThreadProc(funThreadProc pfunThreadProc);
	DWORD	WaitForEvent(DWORD dwSleep = 10, DWORD dwSpace = 10);
	BOOL	WaitForStart();

	BOOL	Create();

	

private:
	pthread_t       m_tid;        // �̱߳�ʶ��
	pthread_attr_t  m_tattr;      // ��ʼ���߳����������
//	BOOL            m_IsContinue; // �̺߳���ѭ����־
	funThreadProc   m_pfunThreadProc; // �̵߳���ں���
//	void *    m_arg;              // ��ں����Ĳ�����

	// �߳̿��Ʊ�־(������)
	BOOL			m_bWorking;   // �߳��Ƿ�����
	BOOL			m_bPausing;   // �߳��Ƿ����
	//BOOL            m_bResumeEvent;
	//BOOL            m_bStopEvent;
	//BOOL            m_bPauseEvent;
//	BOOL			m_bStartEvent;

	// �߳̿��Ʊ�־(����)
	FoxPThreadCond   m_StartCond;
	FoxPThreadCond   m_StopCond;
	FoxPThreadCond   m_PausepCond;
	FoxPThreadCond   m_ResumeCond;
};


