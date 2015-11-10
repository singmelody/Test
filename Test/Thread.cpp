#include "StdAfx.h"
#include "Thread.h"
#include <process.h>
#include <assert.h>
#include "MyLog.h"

#define MS_VC_EXCEPTION 0x406D1388

Thread::Thread(const char* strName) : m_strName(strName)
{
	m_threadID = 0;
	SetStatus(eThread_Ready);
	m_handle = NULL;
}

Thread::~Thread()
{

}

void Thread::Start()
{
	if(GetStatus() != eThread_Ready)
		return;

	SECURITY_ATTRIBUTES attr;
	attr.nLength = sizeof(SECURITY_ATTRIBUTES);
	attr.lpSecurityDescriptor = NULL;
	attr.bInheritHandle = TRUE;

	m_handle = (HANDLE)::_beginthreadex( &attr, 0, ThreadProcess, this, 0, &m_threadID);
	assert( m_handle != NULL );

	{
		typedef struct{
			DWORD	dwType;
			LPCSTR	szName;
			DWORD	dwThreadID;
			DWORD	dwFlags;
		}THREADNAME_INFO;

		THREADNAME_INFO threadName;
		threadName.dwType = 0x100;
		threadName.szName = m_strName.c_str();
		threadName.dwThreadID = m_threadID;
		threadName.dwFlags = 0;

		__try
		{
			//RaiseException( MS_VC_EXCEPTION, 0, sizeof(threadName)/sizeof(DWORD), (ULONG_PTR*)&threadName);
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}

	SetStatus(eThread_Running);
	MyLog::message("Thread[%s] START!", m_strName.c_str());
}

void Thread::Stop()
{
// 	if(GetStatus() != eThread_Running)
// 		return;

	TerminateThread(m_handle, 0);
	CloseHandle( m_handle);

	m_threadID = 0;
	SetStatus(eThread_Exiting);
	MyLog::message("Thread[%s] STOP!", m_strName.c_str());
}

void Thread::Wait()
{
	if( GetStatus() != eThread_Running )
		return;

	::WaitForSingleObject( m_handle, INFINITE);
	m_handle = NULL;

	m_threadID = 0;
	SetStatus(eThread_Ready);

	MyLog::message("Thread[%s] STOP!", m_strName.c_str());
}

bool Thread::IsInThisThread()
{
	if (GetStatus() != eThread_Running && GetStatus() != eThread_Exiting)
		return false;

	if(GetCurrentThreadId() == m_threadID)
		return false;

	return true;
}

void Thread::Sleep(const int64 nMilliSecond)
{
	::Sleep((DWORD)nMilliSecond);
}

unsigned int __stdcall Thread::ThreadProcess(void* ptr)
{
	Thread* pThread = (Thread*)ptr;
	pThread->Run();
	pThread->SetStatus(eThread_Exiting);
	::_endthreadex(0);
	return 0;
}