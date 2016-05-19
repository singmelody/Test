#include "StdAfx.h"
#include "WorldStateGame.h"


WorldStateGame::WorldStateGame(void)
{
}


WorldStateGame::~WorldStateGame(void)
{
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

void WorldStateGame::DecommisionNodeAvatar(WorldAvatar* pAvatar)
{

}

// Node Crash influence following game state : Jumping/Gaming/ExitGame
void WorldStateGame::OnNodeCrashed(int32 nSrvID, bool bUseSHM)
{
	// Node disconnect, can't get player data again, just logout
	if( nSrvID = SERVERID_NULL)
		return;

	TickList& list = m_listAvatars;

	TickNode* pNode = list.GetNext(NULL);
	while( pNode != NULL)
	{
		WorldAvatar* pAvatar = (WorldAvatar*)( pNode->Get() );
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);

		OnNodeCrashed( nSrvID, bUseSHM, pAvatar);
	}
}

void WorldStateGameNode::NotifyChangeSceneFailed(WorldAvatar* pAvatar, int32 failReason)
{
	if(!pAvatar)
		return;

	PacketChangeSceneFail pkt;
	pkt.SetAvatarID( pAvatar->GetAvatarID() );
	pkt.errorID = failReason;
	pAvatar->Send2CurNode(pkt);
}
