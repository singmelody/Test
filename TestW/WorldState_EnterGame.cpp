#include "StdAfx.h"
#include "WorldState_EnterGame.h"
#include "GameObject.h"
#include "MyLog.h"
#include "WorldAvatar.h"
#include "SceneManager.h"

WorldState_EnterGame::WorldState_EnterGame(void)
{
}


WorldState_EnterGame::~WorldState_EnterGame(void)
{
}

void WorldState_EnterGame::Tick(int32 nFrameTime)
{
	TickFCMAvatars(nFrameTime);

	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);
	while (pNode != NULL)
	{
		WorldAvatar* pAvatar = (WorldAvatar*)( pNode->Get() );
		if(pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = pNode->next();

		int32 nPendTime = pAvatar->GetPendingTime();
		if( nPendTime <= 0 )
		{
			MyLog::error("WS_EnterGame Error! Wait create node avatar Time out. id=%d", pAvatar->GetAvatarID());
			OnCreateNodeAvatarFailed( pAvatar, eChangeSceneError_CreateNodeAvatar);
		}
		else
		{
			pAvatar->SubPendingTime(nFrameTime);
		}
	}
}

void WorldState_EnterGame::Destroy(WorldAvatar* pAvatar)
{
	pAvatar->m_bIsDestroy = true;
	ReleaseBillingAndDestroy(pAvatar);
}

void WorldState_EnterGame::OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData)
{
	if ( pAvatar->GetNodeSrvID() == nSrvID || pAvatar->m_nTargetNodeID == nSrvID )
	{
		pAvatar->SetNodeSrvID( SERVERID_NULL );
		pAvatar->m_nTargetNodeID = SERVERID_NULL;
		pAvatar->m_bIsDestroy = true;
		pAvatar->SetCurState( eWS_ExitGame );
	}
}

void WorldState_EnterGame::OnCreateNodeAvatarSucceed(WorldAvatar* pAvatar)
{
	TryCommissionNodeAvatar(pAvatar);
}

void WorldState_EnterGame::OnCreateNodeAvatarFailed(WorldAvatar* pAvatar, int32 nErrorID)
{
	pAvatar->OnEnterGameFailed();
}
