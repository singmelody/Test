#include "StdAfx.h"
#include "WorldState_Gaming.h"
#include "SceneManager.h"
#include "PacketImpl.h"
#include "WorldState_Gaming.h"
#include "WorldAvatar.h"
#include "WorldSceneInfo.h"

WorldState_Gaming::WorldState_Gaming(void)
{
}


WorldState_Gaming::~WorldState_Gaming(void)
{
}

void WorldState_Gaming::OnEnterState(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return;

	WorldStateGame::OnEnterState(pAvatar);

	// When enter gaming state, role data on node now
	if(pAvatar != NULL)
		pAvatar->m_curDataOnSide = eCurDataOnNode;

	// first online, notify teamm
	if(pAvatar->GetWorldTeam() != NULL && !pAvatar->CheckAvatarFlag(eWorldAvatar_Online))
	{
		pAvatar->GetWorldTeam();
		pAvatar->SetAvatarFlag(eWorldAvatar_Online);
	}
}

int32 WorldState_Gaming::ProcessCreateSceneRequest(class PacketCreateSceneRequest* pPkt)
{
	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "WorldState_Gaming::ProcessCreateSceneRequest");

	if(!pAvatar)
		return eCreateSceneError_Creater;

	int32 nSceneID = pPkt->nSceneID;
	int16 nSceneSID = SceneInfo::GetSceneSID( nSceneID );
	int16 nSceneInstanceID = SceneInfo::GetSceneInstanceID( nSceneID );

	WorldSceneInfo* pInfo = SceneMgr.GetWorldSceneInfo( nSceneSID );
	if(!pInfo)
		return eCreateSceneError_SceneInfo;
	
	if(!pInfo->IsCopy())
		return eCreateSceneError_CreateTrunk;

	if(pInfo->HandleEnterSceneExit( nSceneSID, pAvatar))
		return eCreateScene_Succeed;

	if( nSceneInstanceID != SCENE_ID_NULL )
	{
		if( pInfo->IsChallengeCopy() )
		{
			if( pInfo->IsAutoResetProcess() )
			{
				// has auto reset process copy, when old process not exist create a new one
			}
			else
			{
				return eCreateSceneError_SceneNotExist;
			}
		}
		else
		{
			// according to the copy process to continue copy
		}

		// guid copy not exist, judge has guid or not
		if(pInfo->IsGuidCopy() && !pAvatar->IsHasGuid())
		{
			return eCreateSceneError_GuidNotExist;
		}
	}

	return ((WorldSceneInfo_Copy*)pInfo)->HandleCreateCopyRequest( pAvatar, pPkt->SceneProcessBits);
}

void WorldState_Gaming::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketAvatarDataBackup, WorldState_Gaming, PktNode_AvatarDataBackup);

	REG_PACKET_HANDLER( pProc, PacketCommonDataUpdate, WorldState_Gaming, PktNode_CommonDataUpdate);
	REG_PACKET_HANDLER( pProc, PacketCommonDataCreate, WorldState_Gaming, PktNode_CommonDataCreate);
	REG_PACKET_HANDLER( pProc, PacketCommonDataDelete, WorldState_Gaming, PktNode_CommonDataDelete);

	REG_PACKET_HANDLER( pProc, PacketLogOut2Login, WorldState_Gaming, PktLogout2Login);
	REG_PACKET_HANDLER( pProc, PacketChangeSceneRequest, WorldState_Gaming, PktNode_ChangeSceneRequest);
	REG_PACKET_HANDLER( pProc, PacketNodeKickOut, WorldState_Gaming, PktNode_TickOut);
	REG_PACKET_HANDLER( pProc, PacketCreateSceneRequest, WorldState_Gaming, PktNode_CreateSceneRequest);


}

void WorldState_Gaming::PktNode_CreateSceneRequest(class PacketCreateSceneRequest* pPkt)
{
	if(!pPkt)
		return;

	int32 nResult = ProcessCreateSceneRequest( pPkt );
	if( nResult != eCreateScene_Succeed)
		SceneMgr.NotifyCreateSceneResult( pPkt->nNodeID, pPkt->GetAvatarID(), nResult);
}
