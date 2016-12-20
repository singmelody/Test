#include "StdAfx.h"
#include "NodeScenario.h"
#include "NodeAvatar.h"


NodeScenario::NodeScenario(void)
{
}


NodeScenario::~NodeScenario(void)
{
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

}

NodeAvatar* NodeScenario::FindAvatarFromScene(int32 nAvatarID)
{

}
