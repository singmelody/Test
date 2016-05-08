#include "StdAfx.h"

#include "WorldState_Logining.h"
#include "AvatarOnLineManager.h"

WorldState_Logining::WorldState_Logining(void)
{
}


WorldState_Logining::~WorldState_Logining(void)
{
}

void WorldState_Logining::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketUserData, WorldState_Logining, PktDBA_UserData);
	REG_PACKET_HANDLER( pProc, PacketUserDataRegFinish, WorldState_Logining, PktDBA_UserDataReqFinish);
}

void WorldState_Logining::PktDBA_UserData(class PacketUserData* pPkt)
{
	assert(pPkt);
}

void WorldState_Logining::PktDBA_UserDataReqFinish(class PacketUserDataRegFinish* pPkt)
{
	assert(pPkt);

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
		return;

	PacketUserData pkt;
	pkt.SetAvatarID( pAvatar->GetAvatarID() );

	for (int32 i = 0; i < MAX_AVATAR_COUNT_ONE_USER; ++i)
	{
		ParamPool* pPool = pAvatar->GetRoleSet(i);
		if(!pPool)
			continue;

		pkt.AvatarDID = PARAM_GET_VALUE( pPool, avatardid,  (int64)-1);
		pkt.index = i;

		pkt.SyncParam2Gate( pAvatar, true, pPool, Param_Flag_RoleList, Param_Sync_All);
	}
}

void WorldState_Logining::OnEnterStage( WorldAvatar* pAvatar)
{
	WorldState::OnEnterStage(pAvatar);

	// enter the login state, request roll list from dba
	// and reg player 2 online list
	AvatarOnLineManager::Instance().AddAccount(pAvatar);

	PacketSystemConfigRequest pktConfig;
	pktConfig.SetAvatarID( pAvatar->GetAvatarID() );
	pktConfig.AvatarID = pAvatar->GetAvatarID();
	pktConfig.accountLen = uint8(pAvatar->m_account.GetAccountName().size());
	assert(pktConfig.accountLen <= sizeof(pktConfig.account));
	memcpy( pktConfig.account, pAvatar->m_account.GetAccountName().c_str(), pktConfig.accountLen);
	Send2DBA(pktConfig);

	PacketRoleListRequest pkt;
	pkt.SetAvatarID( pAvatar->GetAvatarID() );
	pkt.AccountNameLen = uint8(pAvatar->m_account.GetAccountNmae().size());
	assert( pkt.AccountNameLen <= sizeof(pkt.AccountName));
	memcpy( pkt.AccountName, pAvatar->m_account.GetAccountName().c_str(), pkt.AccountNameLen);

	Send2DBA(PacketRoleListRequest);
}


void WorldState_Logining::OnLeaveStage( WorldAvatar* pAvatar)
{
	WorldState::OnLeaveStage(pAvatar);
}

void WorldState_Logining::Tick(int32 nFrameTime)
{
	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);

	while (pNode != NULL)
	{
		WorldAvatar* pAvatar = (WorldAvatar*)(pNode->Get());
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);

		int32 nPendTime = pAvatar->GetPendingTime();
		if( nPendTime <= 0 )
		{
			MyLog::error("Delete Pending Avatar Data FRrom id=%d", pAvatar->GetAvatarID());

			pNode = list.Remove(pNode);

			DestroyAvatar(pAvatar);
			continue;
		}

		pAvatar->SubPendingTime(nFrameTime);
	}
}

void WorldState_Logining::DestroyAvatar(WorldAvatar* pAvatar)
{
	AvatarOnLineManager::Instance().DelAccount(pAvatar);
	WorldState::DestroyAvatar(pAvatar);
}