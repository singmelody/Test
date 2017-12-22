#include "StdAfx.h"
#include "AIBehavior.h"

Ref_Class(AIBehaviorAction)
AIUNIT_FACTORY_FINISH(AIBehaviorAction)


Ref_Class(AIBehavior)
AIUNIT_FACTORY_FINISH(AIBehavior)
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

AIBehavior* Process(NodeAvatar& avatar)
{
	// 开始技能组CD->begin
	if( m_nCDGroupID > 0 )
	{
		CDComponent* pCDCom = avatar.GetCDComponent();
		if(pCDCom)
			pCDCom->StartCD( m_nCDGroupID, CDType_Skill);
	}

	return this;
}

AIBehaviorAction* GetActionInstance()
{
	if(!m_pAction)
		return NULL;

	AIBehaviorAction* pRes = m_pAction->Clone();
	if(!pRes)
		return NULL;

	pRes->m_ActionID = m_pAction->m_ActionID;
	pRes->ParaAction = m_pAction->ParaAction;
	pRes->Replace = m_pAction->Replace;
	pRes->Clear = m_pAction->Clear;
	pRes->Lock = m_pAction->Lock;
	pRes->ColFrameId = m_pAction->ColFrameId;
	pRes->PartEnableMask = m_pAction->PartEnableMask;

	AIBehaviorAction* resTmp = res;
	AIBehaviorAction* pTmp = 
}

AIBehaviorAction* GetActionbByActIdx(int32 nActIdx)
{
	if(!m_pAction)
		return NULL;

	AIBehaviorAction* pTmp = m_pAction;

	while (true)
	{
		if(AI_DATA_ID(pTmp->GetActionID()) == actIdx)
			return pTmp;

		pTmp = pTmp->next();
		if(!pTmp)
			break;
	}

	return NULL;
}


AIBehavior* AIBehavior::Process( NodeAvatar& avatar)

int32 AIBehavior::GetBehaviorCount()
{
	return m_nBehCount;
}
