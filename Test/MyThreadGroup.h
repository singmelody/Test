#pragma once

#include <vector>
#include "Thread.h"

class MyThreadGroup
{
	typedef std::vector<Thread*> THREAD_VEC;
public:
	MyThreadGroup(void);
	virtual ~MyThreadGroup(void);

	void StartThreads(uint32 nNumOfThread);
	void StopThreads();
	uint32 GetThreadNum() const;
	Thread* GetMinLoadThread();
protected:
	virtual Thread* CreateThread()
	{
		return new Thread();
	}

	virtual void DestroyThread(Thread* pThread)
	{
		SAFE_DELETE(pThread);
	}

protected:
	THREAD_VEC m_Threads;
};

