#include "StdAfx.h"
#include "WorldState_Gaming.h"
#include "SceneManager.h"
#include "PacketImpl.h"
#include "WorldState_Gaming.h"
#include "WorldAvatar.h"
#include "WorldSceneInfo.h"
#include "WorldSceneManager.h"
#include "PacketProcessor.h"
#include "WorldSceneInfoTypes.h"
#include "WorldServer.h"
#include "ParamTypeDef.h"
#include "CommonDataObj.h"

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

	if(pInfo->HandleEnterSceneExist( nSceneSID, pAvatar))
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
		if(pInfo->IsGuildCopy() && !pAvatar->GetGuildID() == INVALID_GUILD_ID )
		{
			return eCreateSceneError_GuildNotExist;
		}
	}

	return ((WorldSceneInfo_Copy*)pInfo)->HandleCreateCopyRequest( pAvatar, pPkt->nSceneProcessBits);
}

void WorldState_Gaming::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketAvatarDataBackup, WorldState_Gaming, PktNode_AvatarDataBackup);

	REG_PACKET_HANDLER( pProc, PacketCommonDataUpdate, WorldState_Gaming, PktNode_CommonDataUpdate);
	REG_PACKET_HANDLER( pProc, PacketCommonDataCreate, WorldState_Gaming, PktNode_CommonDataCreate);
	REG_PACKET_HANDLER( pProc, PacketCommonDataDelete, WorldState_Gaming, PktNode_CommonDataDelete);

	REG_PACKET_HANDLER( pProc, PacketLogout2Login, WorldState_Gaming, PktLogout2Login);
	REG_PACKET_HANDLER( pProc, PacketChangeSceneRequest, WorldState_Gaming, PktNode_ChangeSceneRequest);
	REG_PACKET_HANDLER( pProc, PacketNodeKickOut, WorldState_Gaming, PktNode_TickOut);
	REG_PACKET_HANDLER( pProc, PacketCreateSceneRequest, WorldState_Gaming, PktNode_CreateSceneRequest);


}

void WorldState_Gaming::ExitGame(WorldAvatar* pAvatar)
{
	if(!CheckInState( pAvatar, "WorldState_Gaming::ExitGame"))
		return;

	pAvatar->SetCurState( eWS_ExitGame );
}

void WorldState_Gaming::PktNode_AvatarDataBackup(class PacketAvatarDataBackup* pPkt)
{
	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
		return;

	if(pPkt->nFlag & PacketAvatarDataBackup::FLAG_STORAGE_ONLY)
	{
		//pAvatar->SyncAvatarStorageItemData2DB();
	}
	else
	{
		pAvatar->SyncAvatarData2DB( false, eParam_Sync_ClearDirty);
		if(pPkt->nFlag & PacketAvatarDataBackup::FLAG_EXIT)
			ExitGame(pAvatar);
	}
}

void WorldState_Gaming::PktLogout2Login(class PacketLogout2Login* pPkt)
{
	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "WorldState_Gaming::PktLogout2Login");
	if(!pAvatar)
		return;

	ExitGame( pAvatar );
}

void WorldState_Gaming::PktNode_ChangeSceneRequest(class PacketChangeSceneRequest* pPkt)
{
	assert(pPkt);

	int32 nAvatarID = pPkt->GetAvatarID();

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( nAvatarID, "PktNode_ChangeSceneRequest");
	if(!pAvatar)
		return;

	if(pPkt->GM_Enter != 0)
	{
		WorldScene* pScene = SceneMgr.GetWorldScene(pPkt->nTargetScene);
		if(pScene)
			WorldEnterManager::Instance().HandleEnterScene(pAvatar, pScene);
		return;
	}

	WorldSceneInfo* pInfo = SceneMgr.GetWorldSceneInfo( SceneInfo::GetSceneSID( pPkt->nTargetScene));
	if(!pInfo)
		return;

	Vector3 tarPos( pPkt->x, pPkt->y, pPkt->z);
	Vector3 tarDir( pPkt->dx, pPkt->dy, pPkt->dz);

	pAvatar->SetTargetScenePoint( tarPos );
	pAvatar->SetTargetSceneDir( tarDir );

	int32 nFailReaon = 0;
	bool bTransport = pInfo->TryEnterTargetScene( pAvatar, SceneInfo::GetSceneInstanceID(pPkt->nTargetScene), nFailReaon);
	if(!bTransport)
		NotifyChangeSceneFailed( pAvatar, nFailReaon);
}

void WorldState_Gaming::PktNode_TickOut(class PacketNodeKickOut* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pPlayer = GetWorldAvatar(pPkt);
	if(!pPlayer)
		return;

	WorldSrv.KickOutAvatar(pPlayer);
}

void WorldState_Gaming::PktNode_CommonDataUpdate(class PacketCommonDataUpdate* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
		return;

	CommonDataObject* pObj = pAvatar->OnRecvUpdatePacket(pPkt);
	if(!pObj)
		return;
}

void WorldState_Gaming::PktNode_CommonDataCreate(class PacketCommonDataCreate* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
		return;

	CommonDataObject* pObj = pAvatar->OnRecvCreatePacket(pPkt);
	if(!pObj)
		return;
	
	pObj->SetObjectFlag( pPkt->nFlag );

	if( pObj->NeedSync2DBA() )
		pAvatar->Send2DBA(pPkt);
}

void WorldState_Gaming::PktNode_CommonDataDelete(class PacketCommonDataDelete* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
		return;

	CommonDataObject* pObj = pAvatar->GetCommonDataObj(pPkt);
	if(!pObj)
		return;

	if( pObj->NeedSync2DBA() )
		pAvatar->Send2DBA(pPkt);

	pAvatar->DeleteCommonDataObj( pObj );
}

void WorldState_Gaming::PktNode_CreateSceneRequest(class PacketCreateSceneRequest* pPkt)
{
	if(!pPkt)
		return;

	int32 nResult = ProcessCreateSceneRequest( pPkt );
	if( nResult != eCreateScene_Succeed)
		SceneMgr.NotifyCreateSceneResult( pPkt->nNodeID, pPkt->GetAvatarID(), nResult);


}
