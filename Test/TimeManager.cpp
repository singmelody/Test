#include "StdAfx.h"
#include "TimeManager.h"
#include "Time.h"

TimeManager::TimeManager(void)
{
	m_lastTime = Time::CurrentTime().MilliSecond();
	m_frameTime = 0;
	m_nTotalFrameTime = 0;
	m_frameCount = 0;
	m_uAdditionTimes = 0;
	m_curAdditionTimes = 0;
	m_bAdd = 0;
	m_ProcessLogicTime = 0;
	m_ProcessPacketTime = 0;
	m_FrameTickTime = 0;
}


TimeManager::~TimeManager(void)
{
}

void TimeManager::Tick()
{
	uint64  tTime = Time::CurrentTime().MilliSecond();
	m_frameTime = tTime - m_lastTime;

	if(m_frameTime <= 0)
		m_frameTime = 1;

	m_lastTime = tTime;

	m_nTotalFrameTime += m_frameTime;
	m_frameCount++;

	if (m_uAdditionTimes != 0)
	{
		if(m_uAdditionTimes < 0)
			m_curAdditionTimes = 0;
		else
		{
			m_curAdditionTimes += m_uAdditionTimes;
			m_uAdditionTimes = 0;
		}
	}
}

uint64 TimeManager::CurTime()
{
	return Time::CurrentTime().MilliSecond() + m_curAdditionTimes;
}
