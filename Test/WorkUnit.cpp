#include "StdAfx.h"
#include "WorkUnit.h"
#include "MyRandom.h"
#include "Time.h"
#include <Windows.h>

WorkUnit::WorkUnit(void)
{
	m_nInterval = 100;
	m_bRun = true;
}

WorkUnit::~WorkUnit(void)
{
}

void WorkUnit::Exit()
{
	m_bRun = false;
}

void WorkUnit::Sleep(const Time& time)
{
	::Sleep((DWORD)time.MilliSecond());
}

void WorkUnit::Execute()
{
	m_bRun = true;
	bool bRet = false;

	MyRandom::SRand((int32)Time::CurrentTime().MilliSecond());

	try
	{
		bRet = OnInit();
	}
	catch (...)
	{
		m_bRun = false;
	}

	if(!bRet)
		m_bRun = false;


	while (m_bRun)
	{
		Time curtime = Time::CurrentTime();
		try
		{
			bRet = Process(curtime);
		}
		catch (...)
		{
			if(!OnAssertFail())
			{
				Exit();
				break;
			}
		}

		Time oldTime = curtime;
		curtime = Time::CurrentTime();

		Time sleepTime;
		if( curtime < (oldTime + m_nInterval) )
		{
			sleepTime = m_nInterval + oldTime - curtime;
		}

		if( sleepTime != ZERO_TIME )
		{
			try
			{
				OnSleep(sleepTime);
			}
			catch (...)
			{
				if(!OnAssertFail())
				{
					Exit();
					break;
				}
			}

		}
	}

	try
	{
		OnQuit();
	}
	catch (...)
	{
		return;
	}
}

bool WorkUnit::Process(const Time& curtime)
{
	return true;
}

void WorkUnit::OnSleep(const Time& time)
{
	Sleep(time);
}

