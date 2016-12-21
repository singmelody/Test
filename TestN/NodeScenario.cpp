#include "StdAfx.h"
#include "NodeScenario.h"
#include "NodeAvatar.h"


NodeScenario::NodeScenario(void)
{
}


NodeScenario::~NodeScenario(void)
{
}

bool NodeScenario::InitScenario(NodeScene* pScene, ScenarioFlow* pFlow,UID2AvatarIDMap& map, CreateArgList& listDelay)
{
	m_pScene = pScene;
	m_pScenarioFlow = pFlow;
	m_pScenarioInfo = ;
}

void NodeScenario::DestroyScenario(bool bNotify /*= false*/)
{
	Scenario::DestroyScenario(bNotify);

	if( m_pScene != NULL )
	{
		CombineSceneProcess();
		CombineSceneInstanceProcess();
		SyncLeaveScenario();
		BroadCastEvent();

		if(bNotify)
			BroadCastACEvent();

		if( m_nQuestID > 0 )
		{
			if( m_questOperType == eDefault_ForceAcceptAndFinish || m_questOperType == eForceFinish)
				ForceFinishQuest(m_nQuestID);
		}

		NodeNPC* pNpc = m_mapRemovedNpcs.RemoveFiest();
		while(IsValidMemory(pNpc))
		{
			m_pScene->ExitSceneAndDestroy(pNpc);
			pNpc = m_mapRemovedNpcs.RemoveFirst();
		}
	}
}

NodeAvatar* NodeScenario::FindAvatarInContent(int32 nUID)
{
	if(!m_pMapUID2AvatarID2)
		return NULL;

	int32 nAvatarID = m_pMapUID2AvatarID2->GetAvatarID(nUID);
	if( nAvatarID == 0)
		return NULL;

	return FindAvatarFromScene(nAvatarID);
}

NodeAvatar* NodeScenario::FindAvatarFromScene(int32 nAvatarID)
{
	NodeAvatar* pAvatar = m_pScene->GetSceneAvatar(nAvatarID);
	return pAvatar;
}
