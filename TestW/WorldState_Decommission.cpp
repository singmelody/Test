#include "StdAfx.h"
#include "WorldState_Decommission.h"
#include "SceneManager.h"
#include "WorldAvatar.h"
#include "MyLog.h"
#include "GameObject.h"
#include "ServerManager.h"

WorldState_Decommission::WorldState_Decommission(void)
{
}


WorldState_Decommission::~WorldState_Decommission(void)
{
}

void WorldState_Decommission::Tick(int32 nFrameTime)
{
	TickFCMAvatars(nFrameTime);

	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);

	while ( pNode != NULL )
	{
		WorldAvatar* pAvatar = (WorldAvatar*)(pNode->Get());
		if( pAvatar == NULL )
		{
			pNode = list.Remove(pNode);
			continue;
		}


		pNode = pNode->next();

		int32 nPendingTime = pAvatar->GetPendingTime();
		if( nPendingTime <= 0 )
		{
			if( !pAvatar->m_bTargetNodeAvatarCreated )
			{
				MyLog::error("WS_Decommission Error! Wait create node avatar Time out error id=%d", pAvatar->GetAvatarID());
				OnCreateNodeAvatarFailed( pAvatar, eChangeSceneError_CreateNodeAvatar);

			}
			else
			{
				OnPullAvatarDataFailed(pAvatar);
				continue;
			}
		}
		else
		{
			pAvatar->SubPendingTime(nFrameTime);
		}
	}
}

void WorldState_Decommission::OnPullAvatarDataFinish(WorldAvatar* pAvatar)
{
	WorldStateGame::OnPullAvatarDataFinish(pAvatar);

	if(pAvatar->m_bIsDestroy)
	{
		pAvatar->SetCurState( eWS_WritingDBA );
		DestroyDummyNodeAvatar(pAvatar);
		return;
	}

	TryCommissionNodeAvatar(pAvatar);
}

void WorldState_Decommission::OnPullAvatarDataFailed(WorldAvatar* pAvatar)
{
	MyLog::error("WS_Decommission Error ! Wait pull avatar data Time out Error id = %d", pAvatar->GetAvatarID());

	// notice that there might be avatar created on other node waiting for recving data
	DestroyDummyNodeAvatar( pAvatar );
	ReleaseBillingAndDestroy( pAvatar );
}

void WorldState_Decommission::DestroyAvatar(WorldAvatar* pAvatar)
{
	pAvatar->m_bIsDestroy = true;
}

// node crash only influence Juming/Gaming/ExitGame
void WorldState_Decommission::OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData)
{
	if ( pAvatar->GetNodeSrvID() == nSrvID || pAvatar->m_nTargetNodeID == nSrvID )
	{
		if(bUseSHM && bWaitData)
			pAvatar->SetCurState( eWS_WaitingNodeData );
		else
		{
			pAvatar->SetNodeSrvID( SERVERID_NULL );
			pAvatar->m_nTargetNodeID = SERVERID_NULL;
			pAvatar->m_bIsDestroy = true;
			pAvatar->SetCurState( eWS_ExitGame );
		}
	}
}

void WorldState_Decommission::OnCreateNodeAvatarSucceed(WorldAvatar* pAvatar)
{
	PullAvatarDataFromNode( pAvatar, false);
}

void WorldState_Decommission::OnCreateNodeAvatarFailed(WorldAvatar* pAvatar, int32 nErrorID)
{
	// create node avatar failed , retry to enter the Gaming state
	// if the scene configed can't be reconnect, goto the Gaming state too
	pAvatar->SetCurState(eWS_Gaming);

	NotifyChangeSceneFailed(pAvatar, nErrorID);
}
