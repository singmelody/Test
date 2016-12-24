#include "StdAfx.h"
#include "AIBehavior.h"


AIBehavior::AIBehavior(void)
{
	m_pAction = NULL;
	m_nBehCount = 0;
	m_nBehCount = 0;
	m_nParentID = -1;
	m_nChildIdx = -1;
	m_pGroup = NULL;
	m_nCDGroupID = -1;
	m_nClear = true;

	memset( m_pBehList, 0, sizeof(AIBehavior*)*MAX_CHILD_AI_BEHAVIOR_COUNT);
}


AIBehavior::~AIBehavior(void)
{
	AIBehaviorAction* pCur = m_pAction;
	while (pCur)
	{
		AIBehaviorAction* pNext = pCur->Next();
		AIUnitFactory::Instance().Delete(pCur);
		pCur = pNext;
	}
}

int32 AIBehavior::GetBehaviorCount()
{
	return m_nBehCount;
}
