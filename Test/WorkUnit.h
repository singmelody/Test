#pragma once

#include "Time.h"

class WorkUnit
{
public:
	WorkUnit(void);
	virtual ~WorkUnit(void);

	void Exit();

	void SetInterval(const Time& nInterval)
	{
		m_nInterval = nInterval;
	}

	Time GetInterval() const
	{
		return m_nInterval;
	}

	bool IsAlive() const
	{
		return m_bRun;
	}

	virtual bool OnAssertFail()
	{
		return true;
	}

	static void Sleep(const Time& time);
protected:
	virtual void OnQuit() {}
	void Execute();

	virtual bool OnInit(){ return true; }
	virtual bool Process(const Time& curtime);
	virtual void OnSleep(const Time& time);

	Time m_nInterval;
	volatile bool m_bRun;
};

