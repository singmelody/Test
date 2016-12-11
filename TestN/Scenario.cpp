#include "StdAfx.h"
#include "Scenario.h"


Scenario::Scenario(void)
{
}


Scenario::~Scenario(void)
{
}

bool Scenario::TickScenario(int32 nFrameInterval)
{
	m_curTime += nFrameInterval;
	if( m_curTime < m_TickTime )
		return true;

	if( m_pCurStep != NULL )
	{
		if(!m_pCurStep->TickScenario( this, m_curTime / 1000.0f))
			return false;
	}

	if( m_pCurStep == NULL )
		return false;

	m_curTime = 0.0f;
	return true;
}