#include "StdAfx.h"
#include "WorldState_DataLoading.h"
#include "PacketProcessor.h"

WorldState_DataLoading::WorldState_DataLoading(void)
{
}


WorldState_DataLoading::~WorldState_DataLoading(void)
{
}

void WorldState_DataLoading::OnEnterState(WorldAvatar* pAvatar)
{
	WorldState::OnEnterState(pAvatar);
	
	ParamPool* pPool = pAvatar->GetParamPool();
	if(!pPool)
	{
		MyLog::error("WorldState_DataLoading::OnEnterState No Param Pool");
		return;
	}

	// set login ip
	{
		PARAM_SET_VALUE( pPool, clt_ip, uint32( pAvatar->m_account.GetUserIp()), true);
	}

	// init data loading process
	{
		pAvatar->m_nDataLoadingFlags = 0;
		pAvatar->m_CommonDataMaskFinish = 0;
	}

	pAvatar->m_curDataOnSide = eCurDataOnDBA;

	RequestAvatarDataFromDBA( pAvatar );

	if( WorldServer::UseBilling )
		pAvatar->RequestBillingLogin();
	else
	{
		PARAM_SET_VALUE( pPool, viplevel, uint8(0), true);
		PARAM_SET_VALUE( pPool, left_point, uint8(0), true);
		CreateGateAvatar( *pAvatar, *pPool);
		pAvatar->SetDataLoadingFlags( eDLF_BillingAffirm );
	}
}

void WorldState_DataLoading::OnLeaveState(WorldAvatar* pAvatar)
{
	WorldState::OnLeaveStage(pAvatar);
}

void WorldState_DataLoading::Tick(int32 nFrameTime)
{
	TickList& list = m_listAvatars;

	TickNode* pNode = list.GetNext(NULL);
	while ( pNode != NULL)
	{
		WorldAvatar* pAvatar = (WorldAvatar*)( pNode->Get() );
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);

		if( pAvatar->CheckDataLoadingFlags( eDLF_All ))
		{
			pAvatar->SetCurState( eWS_DataReady );
			continue;
		}

		int32 nPendTime = pAvatar->GetPendingTime();
		if(nPendTime <= 0)
		{
			MyLog::warning("Node Gate Create Pending Time Out");
			MyLog::warning("Avatar Node Gate Affirm = %d", pAvatar->m_nDataLoadingFlags);

			DestroyAvatar(pAvatar);
			continue;
		}

		pAvatar->SubPendingTime(nFrameTime);
	}
}

void WorldState_DataLoading::RequestAvatarDataFromDBA(WorldAvatar* pAvatar)
{
	PacketAvatarData2DBA pkt;
	pkt.SetAvatarID( pAvatar->GetAvatarID());
	pkt.SetAvatarDID( pAvatar->GetAvatarDID() );
	pkt.nFlag = AVATARDATA_SELECT;
	Send2DBA( pkt );
}

bool WorldState_DataLoading::CheckAvatarSceneInfo(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return false;

	ParamPool* pPool = pAvatar->GetParamPool();
	if(!pPool)
		return false;

	int32 nTargetSceneID = pPool->GetValue( "sceneid", (int32)SCENE_ID_NULL);
	int16 nSceneSID = SceneInfo::GetSceneSID(nTargetSceneID);

	bool bRelocate = false;
	const SceneInfo* pInfo = SceneMgr.GetSceneInfo(nSceneSID);

	if(pInfo != NULL)
	{
		f32 x = pPool->GetValue( "downcorrdinatex", 0.0f);
		f32 y = pPool->GetValue( "downcorrdinatey", 0.0f);
		f32 z = pPool->GetValue( "downcorrdinatez", 0.0f);

		if(FLOAT_EQUAL( x, 0.0f) && FLOAT_EQUAL( y, 0.0f) && FLOAT_EQUAL( z, 0.0f))
			bRelocate = true;

		else
		{
			f32 dx = pPool->GetValue( "dirx", 0.0f);
			f32 dy = pPool->GetValue( "diry", 0.0f);
			f32 dz = pPool->GetValue( "dirz", 0.0f);

			pAvatar->SetTargetScenePoint( x, y, z);
			pAvatar->SetTargetSceneDir( dx, dy, dz);
		}
	}
	else
	{
		pInfo = SceneManager::GetSceneInfo( SCENE_ID_DEFAULT);
		if(!pInfo)
			return false;

		nTargetSceneID = SceneInfo::GenSceneID( SCENE_ID_DEFAULT );
	}

	if(bRelocate)
	{
		// get default enter pos
		const EnterPointInfo* pEnter = pInfo->GetEnterPointInfo(0);
		if(!pEnter)
		{
			MyLog::message("Scene Enter Point Error SceneSID [%d]", nSceneSID);
			return false;
		}
		
		pAvatar->SetTargetScenePoint( pEnter->GetRandPos() );
		pAvatar->SetTargetSceneDir( pEnter->Dir );
	}

	pAvatar->m_nTargetSceneID = nTargetSceneID;
	return true;
}

void WorldState_DataLoading::RegPeerPktHandle(PacketProcessor* pProcessor)
{
	REG_PACKET_HANDLER( pProcessor, PacketUserSelectData, WorldState_DataLoading, PktDBA_SelectUserData);
	REG_PACKET_HANDLER( pProcessor, PacketCommonDataInit, WorldState_DataLoading, PktDBA_CommonDataInit);
	REG_PACKET_HANDLER( pProcessor, PacketCommonDataReqFinish, WorldState_DataLoading, PktDBA_CommonDataReqFinish);
	REG_PACKET_HANDLER( pProcessor, PacketGateCreateAvatarRst, WorldState_DataLoading, PktDBA_CreateAvatarRes);
	REG_PACKET_HANDLER( pProcessor, PacketManufactureDataInit, WorldState_DataLoading, PktDBA_ManufactureDataInit);

}

void WorldState_DataLoading::PktDBA_SelectUserData(class PacketUserSelectData* pPkt)
{
	assert( pPkt );

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "PktDBA_SelectUserData");
	if(!pAvatar)
		return;

	pPkt->UpdateParamPool(pAvatar);

	if( pPkt->IsLastPacket() )
	{
		if(!CheckAvatarSceneInfo(pAvatar))
		{
			DestroyAvatar(pAvatar);
			MyLog::message("Avatar Req Enter Game Failed, Error SceneInfo.AvatarID[%d]", pAvatar->GetAvatarID());
			return;
		}

		pAvatar->SetDataLoadingFlags( eDLF_Avatar );
		pAvatar->OnAfterPullDataFromDBA();

		int32 nAvatarID = pAvatar->GetAvatarID();
		int32 nAvatarDID = pAvatar->GetAvatarDID();

		{
			pAvatar->m_CommonDataMaskFinish = 0;

			PacketCommonDataRequest pkt;

			pkt.SetAvatarID( nAvatarID);
			pkt.nAvatarDID = nAvatarDID;

			Send2DBA(pkt);
		}

		{
			PacketNewPlayerMail pkt;
			pkt.nAvatarDID = nAvatarDID;
			pkt.SetAvatarID(nAvatarID);
			Send2DBA(pkt);
		}

		{
			PacketDeletgateQuestDataRequest pkt;
			pkt.SetAvatarID( nAvatarID);
			pkt.nAvatarDID = nAvatarDID;

			Send2DBA(pkt);
		}
	}
}
