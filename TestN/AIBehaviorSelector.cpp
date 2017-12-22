#include "StdAfx.h"
#include "AIBehaviorSelector.h"

AIUNIT_FACTORY_FINISH(AIBehaviorSelector);
AIBehaviorSelector::AIBehaviorSelector(void)
{
	m_bUseWeight = false;
	m_nConditionCount = 0;
}


AIBehaviorSelector::~AIBehaviorSelector(void)
{
}


AIBehavior* AIBehaviorSelector::Clone()
{
	AIBehaviorSelector* pRes = (AIBehaviorSelector*)AIUnitFactory::Instance().New("NewBehaviorSelector");
	if(!pRes)
		return NULL;

	pRes->m_nParentID = m_nParentID;
	pRes->m_nChildIdx = m_nChildIdx;
	pRes->m_nCDGroupID = m_nCDGroupID;
	pRes->m_bUseWeight = m_bUseWeight;


	for (int32 i = 0; i < (int32)m_nConditionCount; ++i)
	{
		AICondition* pResCon = GetCondition(i);
		if(!pResCon)
			continue;

		pResCon = pResCon->Clone();
		pRes->AddCondition(i, resCon);
		pRes->m_nConditionList[i].sWeight = m_nConditionList[i].sWeight;
	}
}

AIBehavior* AIBehaviorSelector::Process(NodeAvatar& avatar)
{
	if(m_nCDGroupID > 0)
	{
		CDComponent * pCDCom = avatar.GetCDComponent();
		if(pCDCom)
		{
			pCDCom->StartCD( m_nGrpIdx, eCDType_Skill);
		}

		if(ServerConfig::DebugAIErrorFlag == 2 || ServerConfig::DebugAIErrorFlag == avatar.GetAvatarID())
		{
			printf("-----------------------------------------------");
		}

		AIBehaviorWeight weight;
	}
}