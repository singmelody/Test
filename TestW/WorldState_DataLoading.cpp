#include "StdAfx.h"
#include "WorldState_DataLoading.h"


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

	}
}




