#include "StdAfx.h"
#include "WorldState_ExitGame.h"
#include "WorldAvatar.h"

WorldState_ExitGame::WorldState_ExitGame(void)
{
}


WorldState_ExitGame::~WorldState_ExitGame(void)
{
}

void WorldState_ExitGame::OnEnterState(WorldAvatar* pAvatar)
{
	WorldStateGame::OnEnterState(pAvatar);

	if(pAvatar->m_bIsDestroy)
	{
		// normal exit
	}
	else
	{
		// to the select scene
		PacketCltLogout pkt;
		pAvatar->SendPacket(&pkt);
	}

	pAvatar->OnAvatarLeaveGame();

	if( pAvatar->m_curDataOnSide == eCurDataOnNode )
		PullAvatarDataFromNode(pAvatar, true);
	else
		pAvatar->SetPendingTime(0);

	pAvatar->CancelAvatarFlag( eWAF_Online );
}

void WorldState_ExitGame::Tick(int32 nFrameTime)
{
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

		pNode = list.GetNext(pNode);

		int32 nPendingTime = pAvatar->GetPendingTime();
		if( nPendingTime <= 0 )
		{
			pAvatar->SetCurState(eWS_WritingDBA);
			continue;
		}
		pAvatar->SubPendingTime(nFrameTime);
	}
}

void WorldState_ExitGame::OnPullAvatarDataFinish(WorldAvatar* pAvatar)
{
	WorldStateGame::OnPullAvatarDataFinish(pAvatar);
	pAvatar->SetCurState(eWS_WritingDBA);
}

void WorldState_ExitGame::DestroyAvatar(WorldAvatar* pAvatar)
{
	pAvatar->m_bIsDestroy = true;
}

void WorldState_ExitGame::OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData)
{
	if( pAvatar->GetNodeSrvID() == nSrvID )
	{
		if( bUseSHM && bWaitData )
			pAvatar->SetCurState( eWS_WaitingNodeData );
		else
		{
			pAvatar->SetNodeSrvID( SERVERID_NULL );
			OnPullAvatarDataFinish(pAvatar);
		}
	}
}
