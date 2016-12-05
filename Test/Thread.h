#pragma once

#include <Windows.h>
#include <string>
#include "NoCopyable.h"

enum Status
{
	eThread_Ready,
	eThread_Running,
	eThread_Exiting,
};

class FunctionBase_Arg0;

class Thread : public NoCopyable
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

	virtual uint32 GetLoad() const{ return 1; }

	static void Sleep(const int64 nMilliSecond);
protected:
	unsigned int m_threadID;

	volatile Status m_status;

	HANDLE m_handle;
	static unsigned int __stdcall ThreadProcess(void* pThread);

	const std::string m_strName;
};

class MyThread : public Thread
{
public:
	explicit MyThread(const std::string& strName = std::string());
	virtual ~MyThread();

	void Init( FunctionBase_Arg0* pFunc);

	inline void Exit() { m_bExit = true; }
	inline bool ShouldExit() const { return m_bExit; }
protected:
	virtual void Run();

	FunctionBase_Arg0*	m_pUpdateFunc;
	volatile bool		m_bExit;
};