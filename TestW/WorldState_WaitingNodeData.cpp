#include "StdAfx.h"
#include "WorldState_WaitingNodeData.h"
#include "PacketProcessor.h"
#include "PacketImpl.h"
#include "MyLog.h"
#include "WorldAvatar.h"
#include "ParamPool.h"
#include "WorldAvatarCommonData.h"
#include "CommonDataObj.h"
WorldState_WaitingNodeData::WorldState_WaitingNodeData(void)
{
}


WorldState_WaitingNodeData::~WorldState_WaitingNodeData(void)
{
}

void WorldState_WaitingNodeData::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncAvatar, WorldState_WaitingNodeData, PktNodeDataSync_AvatarData);
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncCommonData, WorldState_WaitingNodeData, PktNodeDataSync_CommonData);
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncFin, WorldState_WaitingNodeData, PktNodeDataSync_Fin);
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncAllFinish, WorldState_WaitingNodeData, PktNodeDataSync_AllFinish);

}

void WorldState_WaitingNodeData::OnEnterState(WorldAvatar* pAvatar)
{
	assert(pAvatar);
	WorldStateGame::OnEnterState(pAvatar);

	MyLog::message("WorldState_WaitingNodeData::OnEnterState [%s]", pAvatar->GetAccountName());
	pAvatar->SetPendingTime(MAX_WAIT_TIME);

	m_avatarDIDs.insert(pAvatar->GetAvatarDID());
	pAvatar->m_bIsDestroy = true;

	PacketGateDestroyAvatar pkt;
	pAvatar->Send2Gate( &pkt, true);
	pAvatar->SetGateSrvID(SERVERID_NULL);

}

void WorldState_WaitingNodeData::OnLeaveState(WorldAvatar* pAvatar)
{
	assert(pAvatar);
	WorldStateGame::OnLeaveState(pAvatar);


}

void WorldState_WaitingNodeData::Tick(int32 nFrameTime)
{
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

		pNode = list.GetNext(pNode);

		int32 nPendTime = pAvatar->GetPendingTime();
		if(nPendTime <= 0)
		{
			pAvatar->SetCurState(eWS_WritingDBA);
			MyLog::error("WorldState_WaitingNodeData::Tick [%s] time expire", pAvatar->GetAccountName());
			continue;
		}

		pAvatar->SubPendingTime(nFrameTime);
	}
}

void WorldState_WaitingNodeData::DestroyAvatar(WorldAvatar* pAvatar)
{
	pAvatar->SetCurState(eWS_WritingDBA);
}

void WorldState_WaitingNodeData::PktNodeDataSync_AvatarData(class PacketNodeDataSyncAvatar* pPkt)
{
	if(!pPkt)
		return;

	if( m_avatarDIDs.find(pPkt->nAvatarDID) == m_avatarDIDs.end() )
	{
		MyLog::error("WorldState_WaitingNodeData::PktNodeDataSync_AvatarData can not find avatardid[%llu]", pPkt->nAvatarDID);
		return;
	}

	WorldAvatar* pAvatar = GetWorldAvatarByDID(pPkt->nAvatarDID);
	if(!pAvatar)
	{
		MyLog::error("WorldState_WaitingNodeData::PktNodeDataSync_AvatarData can not find avatardid[%llu]", pPkt->nAvatarDID);
		return;
	}

	pAvatar->GetParamPool()->Read( pPkt->GetPacketBuff(), true);
}

void WorldState_WaitingNodeData::PktNodeDataSync_CommonData(class PacketNodeDataSyncCommonData* pPkt)
{
	if(!pPkt)
		return;

	if( m_avatarDIDs.find(pPkt->nAvatarDID) == m_avatarDIDs.end() )
	{
		MyLog::error("WorldState_WaitingNodeData::PktNodeDataSync_CommonData can not find avatardid[%llu]", pPkt->nAvatarDID);
		return;
	}

	WorldAvatar* pAvatar = GetWorldAvatarByDID(pPkt->nAvatarDID);
	if(!pAvatar)
	{
		MyLog::error("WorldState_WaitingNodeData::PktNodeDataSync_CommonData can not find avatardid[%llu]", pPkt->nAvatarDID);
		return;
	}

	WorldAvatarCommonData* pMgr = pAvatar->GetCommonDataManager();
	if(!pMgr)
		return;

	CommonDataObject* pCommonData = pMgr->GetCommonDataObj( CommonDataType( pPkt->nDataType), pPkt->nIdx);
	if(!pCommonData)
	{
		MyLog::error("WorldState_WaitingNodeData::PktNodeDataSync_CommonData can not find commondata \
			avatardid[%llu], type [%d], index [%d]", pPkt->nAvatarDID, pPkt->nDataType, pPkt->nIdx);
		return;
	}

	ParamPool* pPool = pCommonData->GetParamPool();
	if(!pPool)
	{
		MyLog::message("WorldState_WaitingNodeData::PktNodeDataSync_CommonData can not find commondata.ParamPool \
					   avatardid[%llu], type [%d], index [%d]", pPkt->nAvatarDID, pPkt->nDataType, pPkt->nIdx);
		return;
	}

	pPool->Read( pPkt->GetPacketBuff(), true);
}

void WorldState_WaitingNodeData::PktNodeDataSync_Fin(class PacketNodeDataSyncFin* pPkt)
{
	if(!pPkt)
		return;

	if( m_avatarDIDs.find(pPkt->nAvatarDID) == m_avatarDIDs.end() )
	{
		MyLog::error("WorldState_WaitingNodeData::PktNodeDataSync_Fin can not find avatardid[%llu]", pPkt->nAvatarDID);
		return;
	}

	WorldAvatar* pAvatar = GetWorldAvatarByDID(pPkt->nAvatarDID);
	if(!pAvatar)
	{
		MyLog::error("WorldState_WaitingNodeData::PktNodeDataSync_Fin can not find avatardid[%llu]", pPkt->nAvatarDID);
		return;
	}

	if(!CheckInState( pAvatar, "WorldState_WaitingNodeData::PktNodeDataSync_Fin"))
		return;

	MyLog::message("WorldState_WaitingNodeData::PktNodeDataSync_Fin [%s]", pAvatar->GetAccountName());
	pAvatar->SetCurState(eWS_WritingDBA);
}

void WorldState_WaitingNodeData::PktNodeDataSync_AllFinish(class PacketNodeDataSyncAllFinish* pPkt)
{
	if(!pPkt)
		return;

	MyLog::message("WorldState_WaitingNodeData::PktNodeDataSync_AllFinish");

	PacketNodeDataSyncAllFinishRet retPkt;
	PeerSend( pPkt->GetSocketID(), &retPkt);
}
