#include "StdAfx.h"

#include "WorldStage_Logining.h"
#include "AvatarOnLineManager.h"

WorldStage_Logining::WorldStage_Logining(void)
{
}


WorldStage_Logining::~WorldStage_Logining(void)
{
}

void WorldStage_Logining::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketUserData, WorldStage_Logining, PktDBA_UserData);
	REG_PACKET_HANDLER( pProc, PacketUserDataRegFinish, WorldStage_Logining, PktDBA_UserDataReqFinish);
}

void WorldStage_Logining::PktDBA_UserData(class PacketUserData* pPkt)
{
	assert(pPkt);
}

void WorldStage_Logining::PktDBA_UserDataReqFinish(class PacketUserDataRegFinish* pPkt)
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

void WorldStage_Logining::OnEnterStage( WorldAvatar* pAvatar)
{
	WorldStage::OnEnterStage(pAvatar);

	// enter the login state, request roll list from dba
	// and reg player 2 online list
	AvatarOnLineManager::Instance().AddAccount(pAvatar);

	PacketSystemConfigRequest pktConfig;
	pktConfig.SetAvatarID( pAvatar->GetAvatarID() );
	pktConfig.AvatarID = pAvatar->GetAvatarID();
	pktConfig.accountLen = uint8(pAvatar->Account.GetAccountName().size());
	assert(pktConfig.accountLen <= sizeof(pktConfig.account));
	memcpy( pktConfig.account, pAvatar->Account.GetAccountName().c_str(), pktConfig.accountLen);
	Send2DBA(pktConfig);

	PacketRoleListRequest pkt;
	pkt.SetAvatarID( pAvatar->GetAvatarID() );
	pkt.AccountNameLen = uint8(pAvatar->Account.GetAccountNmae().size());
	assert( pkt.AccountNameLen <= sizeof(pkt.AccountName));
	memcpy( pkt.AccountName, pAvatar->Account.GetAccountName().c_str(), pkt.AccountNameLen);

	Send2DBA(PacketRoleListRequest);
}