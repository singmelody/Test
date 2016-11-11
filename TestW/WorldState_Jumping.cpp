#include "StdAfx.h"
#include "WorldState_Jumping.h"
#include "PacketImpl.h"
#include "WorldAvatar.h"
#include "MyLog.h"
#include "PacketProcessor.h"
#include "WorldSceneInfo.h"
WorldState_Jumping::WorldState_Jumping(void)
{
}


WorldState_Jumping::~WorldState_Jumping(void)
{
}

void WorldState_Jumping::OnEnterState(WorldAvatar* pAvatar)
{
	WorldStateGame::OnEnterState(pAvatar);
	pAvatar->SetPendingTime(3600000);
}

void WorldState_Jumping::Tick(int32 nFrameTime)
{
	TickFCMAvatars( nFrameTime );

	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);
	while ( pNode != NULL )
	{
		WorldAvatar* pAvatar = (WorldAvatar*)(pNode->Get());
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = pNode->next();

		int32 nPendTime = pAvatar->GetPendingTime();
		if(nPendTime <= 0)
		{
			MyLog::error("Change Scene Timeout Error id = %d", pAvatar->GetAvatarID());

			if(pAvatar->m_curDataOnSide == eCurDataOnWorld)
				pAvatar->SetCurState(eWS_WritingDBA);
			else
			{
				if(pAvatar->m_bIsDestroy )
				{

				}
				else
				{
					PacketCltSceneReq pkt;
					pkt.nTeleportID = -1;
					pkt.nSceneID = SCENE_ID_NULL;

					pAvatar->SendPacket(&pkt);
				}

				pAvatar->SetCurState(eWS_ExitGame);
			}
		}
		else
		{
			pAvatar->SubPendingTime(nFrameTime);
		}
	}
}

void WorldState_Jumping::DestroyAvatar(WorldAvatar* pAvatar)
{
	pAvatar->m_bIsDestroy = true;
	pAvatar->SetPendingTime(5000);
}

void WorldState_Jumping::OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData)
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

void WorldState_Jumping::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketChangeSceneNotify, WorldState_Jumping, PktNode_ChangeSceneNotify);
	REG_PACKET_HANDLER( pProc, PacketDestroyAvatar, WorldState_Jumping, PktNode_DestroyAvatar);
}

void WorldState_Jumping::PktNode_ChangeSceneNotify(class PacketChangeSceneNotify* pkt)
{
	if(!pkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pkt->GetAvatarID(), "WorldState_Jumping::PktNode_ChangeSceneNotify");
	if(!pAvatar)
		return;

	WorldScene* pScene = GetWorldScene(pkt->nSceneID);
	if(!pScene)
	{
		MyLog::error("Avatar Scene OP:Find Scene Fail id[%d]", pkt->nSceneID);
		return;
	}

	if(pkt->nFlag == PacketChangeSceneNotify::eEnter)
	{
		WorldScene* pOldScene = (WorldScene*)pAvatar->GetScene();
		if(pOldScene != NULL)
		{
			((WorldSceneInfo*)(pOldScene->m_pSceneInfo))->OnLeaveScene(pAvatar, pOldScene);
		}

		pScene->EnterScene(pAvatar);
	}

	PacketGateRouteNotify pNotify;
	pNotify.nFlag = PacketGateRouteNotify::eOpen;
	pNotify.nNodeID = pScene->GetNodeID();

	pAvatar->Send2Gate( &pNotify, true);
	pAvatar->SetCurState( eWS_Gaming );
}

void WorldState_Jumping::PktNode_DestroyAvatar(class PacketDestroyAvatar* pkt)
{
	if(!pkt)
		return;

	int32 nAvatarID = pkt->GetAvatarID();

	MyLog::message("World Recv Destroy Avatar Request [%d]", nAvatarID);
	WorldAvatar* pAvatar = GetWorldAvatar(nAvatarID);
	if(pAvatar == NULL)
	{
		MyLog::error("Destroy Avatar : Find Avatar Fail id [%d]", nAvatarID);
		return;
	}

	DestroyAvatar(pAvatar);
}
