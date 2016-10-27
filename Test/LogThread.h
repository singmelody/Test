#pragma once
#include "Singleton.h"
#include "Thread.h"

class FunctionBase_Arg0;

class LogThread : public MyThread, public Singleton<LogThread>
{
public:
	virtual void Stop();

protected:
	LogThread(void);
	~LogThread(void);

	friend class Singleton<LogThread>;

	void Thread_Update();

	int32 m_frameTime;
	int32 m_logTime;
	uint64 m_lastTime;

private:
	virtual void Init(FunctionBase_Arg0* pFunc);
	virtual void Run();
	inline void Exit() { m_bExit = true; }
	inline bool IsExit() { return m_bExit; }

	FunctionBase_Arg0* m_pUpdateFunc;
	bool m_bExit;
};

