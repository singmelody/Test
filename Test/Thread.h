#pragma once

#include <Windows.h>
#include <string>

#define GSleep(nTime) Thread::Sleep(nTime);

enum Status
{
	eThread_Ready,
	eThread_Running,
	eThread_Exiting,
};

class Thread
{
public:
	explicit Thread( const char* strName = "");
	virtual ~Thread();
	
	void Start();
	virtual void Run(){}
	virtual void Stop();
	void Wait();

	bool IsInThisThread();

	Status GetStatus() { return m_status; }
	void SetStatus(Status nType) { m_status = nType; }

	const char* GetThreadName() { return m_strName.c_str();}

	static void Sleep(const int64 nMilliSecond);
protected:
	unsigned int m_threadID;

	volatile Status m_status;

	HANDLE m_handle;
	static unsigned int __stdcall ThreadProcess(void* pThread);

	const std::string m_strName;
};