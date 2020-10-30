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

// 调试宏
#ifdef _DEBUG
#define Debug_Thread_Print(exp) _Debug_Thread_Print(exp);
#define Debug_Thread_Print_Other(exp) pObject->_Debug_Thread_Print(exp);
#else
#define Debug_Thread_Print(exp) ((void)0)
#define Debug_Thread_Print_Other(exp) ((void)0)
#endif

// 宏定义
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
	pthread_t       m_tid;        // 线程标识符
	pthread_attr_t  m_tattr;      // 初始化线程所需的属性
//	BOOL            m_IsContinue; // 线程函数循环标志
	funThreadProc   m_pfunThreadProc; // 线程的入口函数
//	void *    m_arg;              // 入口函数的参数表

	// 线程控制标志(非阻塞)
	BOOL			m_bWorking;   // 线程是否启动
	BOOL			m_bPausing;   // 线程是否挂起
	//BOOL            m_bResumeEvent;
	//BOOL            m_bStopEvent;
	//BOOL            m_bPauseEvent;
//	BOOL			m_bStartEvent;

	// 线程控制标志(阻塞)
	FoxPThreadCond   m_StartCond;
	FoxPThreadCond   m_StopCond;
	FoxPThreadCond   m_PausepCond;
	FoxPThreadCond   m_ResumeCond;
};


