#pragma once

#include "Singleton.h"

class TimeManager : public Singleton<TimeManager>
{
public:
	TimeManager(void);
	~TimeManager(void);

	void Tick();

	uint64 CurTime();

	int32 FrameTime()
	{
		return m_frameTime;
	}

	inline void FrameTime(int32 nTime)
	{
		m_frameTime = nTime;
		if( m_frameTime <= 0 )
			m_frameTime = 1;
	}
protected:
	uint64	m_lastTime;
	int32	m_frameTime;
	uint64	m_nTotalFrameTime;
	uint64  m_frameCount;
	int64	m_uAdditionTimes;
	int64	m_curAdditionTimes;
	bool	m_bAdd;
	int64	m_ProcessLogicTime;
	int64	m_ProcessPacketTime;
	int64	m_FrameTickTime;
};

