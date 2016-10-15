#include "StdAfx.h"
#include "WorldState_Logined.h"
#include "PacketProcessor.h"
#include "PacketImpl.h"
#include "WorldAvatar.h"
#include "ParamTypeDef.h"
#include "MyLog.h"
#include "ParamPool.h"
#include "AvatarOnLineManager.h"
#include "Time.h"

WorldState_Logined::WorldState_Logined(void)
{
}


WorldState_Logined::~WorldState_Logined(void)
{
}

void WorldState_Logined::OnEnterState(WorldAvatar* pAvatar)
{
	WorldState::OnEnterState(pAvatar);
}

void WorldState_Logined::OnLeaveState(WorldAvatar* pAvatar)
{
	WorldState::OnLeaveState(pAvatar);
}

void WorldState_Logined::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketCreateAvatar, WorldState_Logined, PktGate_CreateAvatar);
	REG_PACKET_HANDLER( pProc, PacketCreateAvatarRes, WorldState_Logined, PktGate_CreateAvatarRes);

	REG_PACKET_HANDLER( pProc, PacketDelCharacter, WorldState_Logined, PktGate_DelCharacter);
	REG_PACKET_HANDLER( pProc, PacketDelCharacterFinish, WorldState_Logined, PktGate_DelCharacterRes);

	REG_PACKET_HANDLER( pProc, PacketCltSelectAvatar, WorldState_Logined, PktGate_CltSelectAvatar);

}

void WorldState_Logined::PktGate_CreateAvatar(class PacketCreateAvatar* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "WorldState_Logined::PktGate_CreateAvatar() Called");
	if(!pAvatar)
		return;

	if(pPkt->nAvatarTitleLen > sizeof(pPkt->avatarTitle)) // pass the gate filter, can't be happen normal
	{
		MyLog::error("PktGate_CreateAvatar Bad Title = [%d]", pPkt->nAvatarTitleLen);
		return;
	}

	const std::string strTitle( pPkt->avatarTitle, pPkt->nAvatarTitleLen);

	MyLog::message("User Req Create Avatar Account [%s] Title [%s]", pAvatar->m_account.GetAccountName().c_str(), strTitle.c_str());

	int32 resPos = -1;
	for (int32 i = 0; i < MAX_AVATAR_COUNT_ONE_USER; ++i)
	{
		ParamPool* pPool = pAvatar->GetRoleSet(i);
		if(pPool == NULL)
		{
			pAvatar->m_account.CreateRoleSet( i, PARAM_ID(pPkt->paramTypeID), PARAM_DATA_ID(pPkt->paramTypeID));
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


	pPkt->accountLen = uint8(pAvatar->Account.GetAccountName().size());
	if(pPkt->accountLen > sizeof(pPkt->account))
	{
		MyLog::error("Bad Account Len = [%d]", pPkt->accountLen);
		PacketCreateAvatarsRes pkt;
		pkt.flag = CreateAvatarRes_Fail;
		pkt.index = -1;
		pAvatar->Send2Gate( &pkt, true);
		return true;
	}

	const std::string strAccount( pPkt->account, pPkt->nAccountLen);

	memcpy( pPkt->account, pAvatar->m_account.GetAccountName(), pPkt->nAccountLen);

	PARAM_SET_VALUE( pPool, faceid, pPkt->faceid, false);
	PARAM_SET_VALUE( pPool, hairid, pPkt->hairid, false);
	PARAM_SET_VALUE( pPool, eyeid, pPkt->eyeid, false);
	PARAM_SET_VALUE( pPool, skincolor, pPkt->skincolor, false);
	PARAM_SET_VALUE( pPool, tattoo, pPkt->tattoo, false);
	PARAM_SET_VALUE( pPool, beard_eyeshadow, pPkt->beard_eyeshadow, false);
	PARAM_SET_VALUE( pPool, morph_1, pPkt->morph_1, false);
	PARAM_SET_VALUE( pPool, morph_2, pPkt->morph_2, false);
	PARAM_SET_VALUE( pPool, morph_3, pPkt->morph_3, false);
	PARAM_SET_VALUE( pPool, fashionid, pPkt->fashionid, false);
	PARAM_SET_VALUE( pPool, fashioncolor, pPkt->fashioncolor, false);
	PARAM_SET_VALUE( pPool, lipcolor, pPkt->lipcolor, false);

	PARAM_SET_VALUE( pPool, title, strTitle.c_str(), false);
	PARAM_SET_VALUE( pPool, accountname, strAccount.c_str(), false);

	pPkt->AvatarDID = pAvatar->GenGalaxyUID();

	PARAM_SET_VALUE( pPool, avatardid, pPkt->AvatarDID, false);
	PARAM_SET_VALUE( pPool, create_time, Time::CurrentTime().Second(), false);

	pPkt->index = resPos;
	pAvatar->SetState( eGAS_Base_CltCreateAvatar);

	// create avatar log
	Send2DBA(pPkt);
}

void WorldState_Logined::PktGate_CreateAvatarRes(class PacketCreateAvatarRes* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);

	if(!pAvatar || !pAvatar->CheckState(eGAS_Base_CltCreateAvatar))
		return;

	MyLog::message("User Reqw Create Avatar.Account[%s] Result[%d]", pAvatar->GetAccountName(), pPkt->nFlag);

	if(pPkt->nFlag != CreateAvatarRes_Succ)
	{
		pAvatar->m_account.DestroyRoleSet( pPkt->nIdx);
		pAvatar->Send2Gate( &pPkt, true);
		return;
	}

	ParamPool* pPool = pAvatar->GetRoleSet(pPkt->nIdx);
	if(!pPool)
	{
		MyLog::error("there is error in create avatar !!");

		PacketCreateAvatarRes pPkt;
		pPkt.nFlag = CreateAvatarRes_Fail;
		pPkt.nIdx = -1;
		pAvatar->Send2Gate( &pPkt, true);
		return;
	}

	{
		// tell clt create success
		PacketCreateAvatarRes pkt;
		pkt.nFlag = CreateAvatarRes_Success;
		pkt.nIdx = -1;
		pAvatar->Send2Gate( &pkt, true);

		pAvatar->ClearState( eGAS_Base_CltCreateAvatar );
	}

	{
		PacketUserData pkt;

		pkt.AvatarDID = pPool->GetValue("avatardid", (int64)-1);
		pkt.index = pPkt->index;

		pkt.SyncParam2Gate( pAvatar, true, pPool, eParam_Flag_RoleList, eParam_Sync_All);
	}
}

void WorldState_Logined::PktGate_DelCharacter(class PacketDelCharacter* pPkt)
{
	if(!pPkt || pPkt->nIdx < 0 || pPkt->nIdx >= MAX_AVATAR_COUNT_ONE_USER)
		return;

	WorldAvatar* pAvatar = GetWorldAvatarAndCheckStage( pPkt->GetAvatarID(), "WorldState_Logined::PktGate_DelAvatar() Called");
	if(!pAvatar)
		return;

	MyLog::message("User Req Del Avatar.Account[%s] DelIndex[%d]", pAvatar->GetAccountName(), pkt->index));
	ParamPool* pPool = pAvatar->GetRoleSet(pkt->index);
	if(!pPool)
		return;

	int64 nAvatarDID = PARAM_GET_VALUE( pPool, avatardid, (int64)-1);
	if(nAvatarDID == -1)
		return;

	std::string name = PARAM_GET_STRING( pPool, accountname, "");
	if(name.length() == 0)
		return;

	PacketDelCharacater2DBA pkt;
	pkt.SetAvatarID(pAvatar->GetAvatarID());
	pkt.nAvatarDID = nAvatarDID;
	pkt.nAccountID = UtilID::CreateFromString(name.c_str());
	Send2DBA(&pkt);
}

void WorldState_Logined::PktGate_DelCharacterRes(class PacketDelCharacterFinish* pPkt)
{
	if(!pPkt)
		return;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
		return;

	if (pPkt->nFlag == eDelete_Avatar_Success)
	{
		ParamPool* pPool = NULL;
		for(int32 i = 0; i < MAX_AVATAR_COUNT_ONE_USER; ++i)
		{
			pPool = pAvatar->GetRoleSet(i);
			if(!pPool)
				continue;

			int64 nAvatarDID = pPool->GetValue( "avatardid", int64(-1));
			if( nAvatarDID == pPkt->nAvatarDID )
			{
				pAvatar->m_account.DestroyRoleSet(i);
				break;
			}

		}
	}

	pAvatar->Send2Gate( pPkt,  true);
}

void WorldState_Logined::PktGate_CltSelectAvatar(class PacketCltSelectAvatar* pPkt)
{
	if(!pPkt)
		return;

	uint8 nReason = uint8(-1);

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt);
	if(!pAvatar)
	{
		MyLog::error("PktGate_CltSelectAvatar Can not find Avatar by id = [%d]", pPkt->GetAvatarID() );
		nReason = uint8(PacketCltSelectAvatarFailed::eFR_Unkown);
	}
	else
	{
		if(!CheckInStage(pAvatar, "WorldState_Logined::PktGate_CltSelectAvatar") )
		{
			nReason = uint8(PacketCltSelectAvatarFailed::eFR_Unkown);
		}
		else
		{
			ParamPool* pPool = pAvatar->m_account.GetRoleSet( pPkt->nAvatarIdx );
			if(!pPool)
			{
				nReason = uint8(PacketCltSelectAvatarFailed::eFR_BadIndex);
				MyLog::message("Avatar Req Enter Game Failed: Select UserDataParam Wrong account=[%s] index=[%d]", pAvatar->GetAccountName(), pAvatar->GetAvatarDID());
			}
			else
			{
				int64 nAvatarDID = PARAM_GET_VALUE( pPool, avatardid, (int64)0);
				if(PARAM_GET_VALUE( pPool, ban_game_date, uint32(0)) > Time::CurrentTime().Second())
				{
					nReason = uint8(PacketCltSelectAvatarFailed::eFR_AvatarBanded);
					MyLog::message("Avatar Req Enter Game Failed avatar was banned account=[%s] avatardid = [%lld]", pAvatar->GetAccountName(), pAvatar->GetAvatarDID());
				}
				else
				{
					pAvatar->m_account.PickRoleSet( pPkt->nAvatarIdx );

					pAvatar->SetParamPool( pPool );
					pAvatar->SetAvatarDID(nAvatarDID);
					pAvatar->SetCurState( eWS_DataLoading );
				}
			}
		}
	}

	if( nReason != uint8(-1))
	{
		PacketCltSelectAvatarFail failPkt;
		failPkt.SetAvatarID( pPkt->GetAvatarID() );
		failPkt.nReason = nReason;
		failPkt.SetPacketType(ePacketType_GateProc);
		PeerSend( pPkt->GetSocketID(), &failPkt);
	}
}

void WorldState_Logined::DestroyAvatar(WorldAvatar* pAvatar)
{
	AvatarOnLineManager::Instance().DelAccount(pAvatar);
	WorldState::DestroyAvatar(pAvatar);
}

void WorldState_Logined::DestroyAvatar(WorldAvatar* pAvatar)
{

}
