#pragma once

#include "Singleton.h"

class TimeManager : public Singleton<TimeManager>
{
public:
	TimeManager(void);
	~TimeManager(void);

	void Tick();

	inline uint64 CurTime();
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

