#include "StdAfx.h"
#include "WorldStage_Logined.h"


WorldStage_Logined::WorldStage_Logined(void)
{
}


WorldStage_Logined::~WorldStage_Logined(void)
{
}

void WorldStage_Logined::OnEnterStage(WorldAvatar* pAvatar)
{
	WorldStage::OnEnterStage(pAvatar);
}

void WorldStage_Logined::OnLeaveStage(WorldAvatar* pAvatar)
{
	WorldStage::OnLeaveStage(pAvatar);
}

void WorldStage_Logined::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketCreateAvatar, WorldStage_Logined, PktGate_CreateAvatar);
	REG_PACKET_HANDLER( pProc, PacketCreateAvatarRes, WorldStage_Logined, PktGate_CreateAvatarRes);

	REG_PACKET_HANDLER( pProc, PacketDelAvatar, WorldStage_Logined, PktGate_DelAvatar);
	REG_PACKET_HANDLER( pProc, PacketDelAvatarRes, WorldStage_Logined, PktGate_DelAvatarRes);

	REG_PACKET_HANDLER( pProc, PacketCltSelectAvatar, WorldStage_Logined, PktGate_CltSelectAvatar);

}

void WorldStage_Logined::PktGate_CreateAvatar(class PacketCreateAvatar* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "WorldStage_Logined::PktGate_CreateAvatar() Called");
	if(!pAvatar)
		return;

	if(pPkt->avatarTitleLen > sizeof(pPkt->avatgarTitle)) // pass the gate filter, can't be happen normal
	{
		MyLog::error("PktGate_CreateAvatar Bad Title = [%d]", pPkt->avatartTitleLen);
		return;
	}

	const std::string strTitle( pPkt->avatarTitle, pPkt->avatarTitleLen);

	MyLog::message("User Req Create Avatar Account [%s] Title [%s]", pAvatar->Account.GetAccountName().c_str(), );

	int32 resPos = -1;
	for (int32 i = 0; i < MAX_AVATAR_COUNT_ONE_USER; ++i)
	{
		ParamPool* pPool = pAvatar->getRoleSet(i);
		if(pPool == NULL)
		{
			pAvatar->Account.CreateRoleSet( i, PARAM_ID(pPkt->paramTypeID), PARAM_DATA_ID(pPkt->paramTypeID));
			resPos = i;
			break;
		}
	}

	if( -1 == resPos )
	{
		PacketCreateAvatarRes pkt;
		pkt.flag = CreatAvatarRes_Fail_MaxCount;
		pkt.index = -1;
		pAvatar->Send2Gate( &pkt, true);
		return;
	}

	ParamPool* pPool = pAvatar->GetRoleSet(resPos);
	if(pPool)
	{
		PacketCreateAvatarRes pkt;
		pkt.flag = CreatAvatarRes_Fail;
		pkt.index = -1;
		pAvatar->Send2Gate( &pkt, true);
		return;
	}

	if(pPkt->checkdata)
	{
		bool bDirtyCheck = DIYDataManager::Instance().CheckDIYDataValid(pPkt);
		if(bDirtyCheck)
		{
			MyLog::error("Bad DIYData!");
			PacketCreateAvatarRes pkt;
			pkt.flag = CreatAvatarRes_Fail;
			pkt.index = -1;
			pAvatar->Send2Gate( &pkt, true);
		}
	}

	balbalbala
}

void WorldStage_Logined::PktGate_CreateAvatarRes(class PacketCreateAvatarRes* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);

	if(!pAvatar || !pAvatar->CheckState(eGAS_Base_CltCreateAvatar))
		return;

	MyLog::message("User Reqw Create Avatar.Account[%s] Result[%d]", pAvatar->GetAccountName(), pPkt->flag);

	if(pPkt->flag != CreateAvatarRes_Succ)
	{
		pAvatar->Account.DestroyRoleSet( pPkt->index);
		pAvatar->Send2Gate( &pPkt, true);
		return;
	}

	ParamPool* pPool = pAvatar->GetRoleSet(pPkt->index);
	if(!pPool)
	{
		MyLog::error("there is error in create avatar !!");

		PacketCreateAvatarRes pPkt;
		pPkt.flag = CreateAvatarRes_Fail;
		pPkt.index = -1;
		pAvatar->Send2Gate( &pPkt, true);
		return;
	}

	{
		// tell clt create success
		PacketCreateAvatarRes pkt;
		pkt.flag = CreateAvatarRes_Success;
		pkt.index = -1;
		pAvatar->Send2Gate( &pkt, true);

		pAvatar->ClearState( eGAS_Base_CltCreateAvatar );
	}

	{
		PacketUserData pkt;

		pkt.AvatarDID = pPool->GetValue("avatardid", (int64)-1);
		pkt.index = pPkt->index;

		pkt.SyncParam2Gate( pAvatar, true, pPool, Param_Flag_RoleList, Param_Sync_All);
	}
}

void WorldStage_Logined::PktGate_DelAvatar(class PacketDelAvatar* pPkt)
{
	if(!pPkt || pPkt->index < 0 || pPkt->index >= MAX_AVATAR_COUNT_ONE_USER)
		return;

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "WorldStage_Logined::PktGate_DelAvatar() Called");
	if(!pAvatar)
		return;

	MyLog::message("User Req Del Avatar.Account[%s] DelIndex[%d]", pAvatar->GetAccountName, pkt->index));
	ParamPool* pPool = pAvatar->GetRoleSet(pkt->index);
	if(!pPool)
		return;

	int64 nAvatarDID = PARAM_GET_VALUE( pPool, avatardid, (int64)-1);
	if(avatardid == -1)
		return;

	std::string name = PARAM_GET_STRING( pPool, accountname, "");
	if(name.length() == 0)
		return;

	PacketDelCharacater2DBA pkt;
	pkt.SetAvatarID(pAvatar->GetAvatarID());
	pkt.avatardid = nAvatarDID;
	pkt.accountid = UtilID::CreateFromString(name.c_str());
	Send2DBA(&pkt);
}

void WorldStage_Logined::PktGate_DelAvatarRes(class PacketDelAvatarRes* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
		return;

	if (pPkt->nFlag == Delete_Avatar_Success)
	{
		ParamPool* pPool = NULL;
		for(int32 i = 0; i < MAX_AVATAR_COUNT_ONE_USER; ++i)
		{
			pPool = pAvatar->GetRoleSet(i);
			if(!pPool)
				continue;

			int64 nAvatarDID = pPool->GetValue( "avatardid", int64(-1));
			if( nAvatarDID == pPkt->avatarDID )
			{
				pAvatar->Account.DestroyRoleSet(i);
				break;
			}

		}
	}

	pAvatar->Send2Gate( pPkt,  true);
}

void WorldStage_Logined::PktGate_CltSelectAvatar(class PacketCltSelectAvatar* pPkt)
{
	if(!pPkt)
		return;

	uint8 nReason = uint8(-1);

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
	{
		MyLog::error("");
	}
}

