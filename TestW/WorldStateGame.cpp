#include "StdAfx.h"
#include "WorldStateGame.h"
#include "WorldAvatar.h"
#include "PacketImpl.h"
#include "WorldServer.h"
#include "WorldAvatarManager.h"

WorldStateGame::WorldStateGame(void)
{
}


WorldStateGame::~WorldStateGame(void)
{
}

void WorldStateGame::OnEnterState(WorldAvatar* pAvatar)
{
	WorldState::OnEnterState(pAvatar);

	if(!pAvatar->Account.IsFCMAccount())
		return;

	m_listFCMAvatars.insert(pAvatar);

	if(pAvatar->Account.NeedNoticeFCMState())
	{
		pAvatar->TickFCM(0);
	}
}

void WorldStateGame::OnLeaveState(WorldAvatar* pAvatar)
{
	WorldState::OnLeaveState(pAvatar);

	if(!pAvatar->Account.IsFCMAccount())
		return;

	m_listFCMAvatars.erase(pAvatar);
}

void WorldStateGame::TryCommissionNodeAvatar(WorldAvatar* pAvatar)
{
	if( CommisionNodeAvatar( pAvatar ))
	{
		// enter jumping state now
	}
	else
	{
		pAvatar->SetCurState( eWS_WritingDBA);
		DestroyDummyNodeAvatar(pAvatar);
	}
}

bool WorldStateGame::CommisionNodeAvatar(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return false;

	WorldScene* pScene = GetWorldScene( pAvatar->m_nTargetSceneID );
	if(!pScene)
		return false;

	int32 nNodeID = pScene->GetNodeID();
	pAvatar->SetNodeSrvID(nNodeID);

	if(!pAvatar->PushAllData2CurNode())
		return false;

	pAvatar->m_nTargetNodeID = SERVERID_NULL;
	pAvatar->m_nTargetSceneID = SCENE_ID_NULL;

	PacketAvatarCommission pkt;
	pkt.info = 0;
	pAvatar->Send2CurNode(pkt);
	pAvatar->SetCurState( eWS_Jumping );
	return true;
}

// just can be called in the OnPullAvatarDataFinish function
void WorldStateGame::DecommisionNodeAvatar(WorldAvatar* pAvatar)
{
	if (!pAvatar)
		return;

	WorldScene* pScene = (WorldScene*)pAvatar->GetScene();
	if(pScene)
	{
		pScene->ExitScene(pAvatar);

		if(pAvatar->HasGateAvatar())
		{
			PacketGateRouteNotify pkt;
			pkt.nFlag = PacketGateRouteNotify::eClose;
			pkt.nNodeID = -1;

			pAvatar->Send2Gate( &pkt, true);
		}
	}

	int32 nNodeSrvID = pAvatar->GetNodeSrvID();
	if( nNodeSrvID != SERVERID_NULL )
	{
		PacketAvatarDecommision pkt;
		pkt.SetAvatarID( pAvatar->GetAvatarID() );
		Send2Node( pkt, nNodeSrvID);

		pAvatar->SetNodeSrvID( SERVERID_NULL );
	}
}

void WorldStateGame::OnPullAvatarDataFinish(WorldAvatar* pAvatar)
{
	pAvatar->OnAfterPullDataFromNode();
	DecommisionNodeAvatar(pAvatar);
}

void WorldStateGame::PullAvatarDataFromNode(WorldAvatar* pAvatar, bool bLogout)
{

}

// Node Crash influence following game state : Jumping/Gaming/ExitGame
void WorldStateGame::OnNodeCrashed(int32 nSrvID, bool bUseSHM)
{
	// Node disconnect, can't get player data again, just logout
	if( nSrvID == SERVERID_NULL )
		return;

	bool bWaitData = true;
	{
		if (Servers.IsWarServer(nSrvID))
		{
			if(!Servers.GetWarWorldInfo())
				bWaitData = false;
		}
	}

	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);
	while( pNode != NULL )
	{
		WorldAvatar* pAvatar = (WorldAvatar*)( pNode->Get() );
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);

		OnNodeCrashed( nSrvID, bUseSHM, pAvatar, bWaitData);
	}
}

void WorldStateGame::ReleaseBillingAndDestroy(WorldAvatar* pAvatar)
{
	if (WorldServer::bUseBilling)
	{
		pAvatar->SetCurState(eWS_Billing);
	}
	else
		AvatarMgr.RemoveWorldAvatar(pAvatar);
}

void WorldStateGame::DestroyDummyNodeAvatar(WorldAvatar* pAvatar)
{
	PacketNodeDestroyAvatar pkt;

	pkt.SetAvatarID( pAvatar->GetAvatarID() );
	Send2Node( pkt, pAvatar->m_nTargetNodeID);
}

void WorldStateGame::TickFCMAvatars(int32 nFrameTime)
{

}

void WorldStateGameNode::NotifyChangeSceneFailed(WorldAvatar* pAvatar, int32 failReason)
{
	if(!pAvatar)
		return;

	PacketChangeSceneFail pkt;
	pkt.SetAvatarID( pAvatar->GetAvatarID() );
	pkt.nErrorID = failReason;
	pAvatar->Send2CurNode(pkt);
}
