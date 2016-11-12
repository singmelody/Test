#include "StdAfx.h"
#include "WorldState_WritingDBA.h"
#include "MyLog.h"
#include "PacketProcessor.h"
#include "WorldAvatar.h"
#include "AvatarOnLineManager.h"
#include "WorldServer.h"
#include "ParamTypeDef.h"

WorldState_WritingDBA::WorldState_WritingDBA(void)
{
}


WorldState_WritingDBA::~WorldState_WritingDBA(void)
{
}

void WorldState_WritingDBA::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketAvatarDecommision, WorldState_WritingDBA, PktDBADecommision);
}

void WorldState_WritingDBA::OnEnterState(WorldAvatar* pAvatar)
{
	assert(pAvatar);

	WorldStateGame::OnEnterState(pAvatar);

	if(pAvatar->m_curDataOnSide == eCurDataOnDBA )
		pAvatar->SetPendingTime(0);
	else
	{
		pAvatar->SetPendingTime(MAX_WAIT_TIME);
		pAvatar->SyncAvatarData2DB( true, eParam_Sync_ClearDirty);
	}
}

void WorldState_WritingDBA::Tick(int32 nFrameTime)
{
	TickFCMAvatars(nFrameTime);

	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);
	while ( pNode != NULL)
	{
		WorldAvatar* pAvatar = (WorldAvatar*)pNode->Get();
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);
		int32 nPendingTime = pAvatar->GetPendingTime();
		if (nPendingTime <= 0)
		{
			MyLog::message("WorldState_WaitingDBA::Tick [%s] Time expire", pAvatar->GetAccountName());
			ReleaseBillingAndDestroy(pAvatar);
		}

		pAvatar->SubPendingTime(nFrameTime);
	}

}

void WorldState_WritingDBA::DestroyAvatar(WorldAvatar* pAvatar)
{
	ReleaseBillingAndDestroy(pAvatar);
}

void WorldState_WritingDBA::PktDBADecommision(class PacketAvatarDecommision* pPkt)
{
	assert(pPkt);

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "PktDBADecommision");
	if(!pAvatar)
		return;

	AvatarOnLineManager::Instance().DelAvatar(pAvatar);

	MyLog::message("Avatar Logout.AvatarID[%d] Account[%s]", pAvatar->GetAvatarID(), pAvatar->GetAccountName());

	pAvatar->ReleaseComponent();
	pAvatar->ReleaseCommonDataConts();

	ParamPool* pPool = pAvatar->GetParamPool();
	if(pPool != NULL)
	{
		pAvatar->Account.SetRecentRoleSet( pPool );
		pAvatar->ClearParamPool();
	}

	if(WorldServer::bUseBilling)
		pAvatar->SetCurState(eWS_Billing);
	else
	{
		if(pAvatar->m_bIsDestroy)
			pAvatar->SetCurState(eWS_Billing);
		else
		{
			pAvatar->CreateComponent();
			pAvatar->m_nChangeSceneState = 0;
			pAvatar->SetCurState( eWS_Logining );
		}
	}

}
