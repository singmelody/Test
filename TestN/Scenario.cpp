#include "StdAfx.h"
#include "Scenario.h"
#include "MyLog.h"

#include "ScenarioManager.h"

Scenario::Scenario(void)
{
	m_fTickTime = 500.0f;
	m_fCurTime = 0.0f;
}


Scenario::~Scenario(void)
{
}

bool Scenario::StartScenario()
{
	SetCurStep(0);
	return true;
}

void Scenario::DestroyScenario(bool bNotify /*= false*/)
{

}

bool Scenario::TickScenario(int32 nFrameInterval)
{
	m_fCurTime += nFrameInterval;
	if( m_fCurTime < m_fTickTime )
		return true;

	if( m_pCurStep != NULL )
	{
		if(!m_pCurStep->TickScenario( this, m_fCurTime / 1000.0f))
			return false;
	}

	if( m_pCurStep == NULL )
		return false;

	m_fCurTime = 0.0f;
	return true;
}

bool Scenario::SetCurStep(int32 nStepID)
{
	ScenarioStepInfo* pNewStep = m_pScenarioInfo->GetStepInfo(nStepID);
	if(!pNewStep)
	{
		MyLog::error("Scenario[%d] StepInfo[%d] is NULL!", m_pScenarioInfo->nScenarioID, nStepID);
		return false;
	}

	if( m_pCurStep != NULL )
		m_pCurStep->LeaveScenario(this);

	m_pCurStep = pNewStep;

	m_pCurStep->EnterScenario(this);
	return true;
}
