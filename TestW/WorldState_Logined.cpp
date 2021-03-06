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
#include "AvatarDefine.h"
#include "DIYDataManager.h"

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
			pAvatar->m_account.CreateRoleSet( i, PARAM_ID(pPkt->nParamTypeID), PARAM_DATA_ID(pPkt->nParamTypeID));
			resPos = i;
			break;
		}
	}

	if( -1 == resPos )
	{
		PacketCreateAvatarRes pkt;
		pkt.nFlag = eCreateAvatarRes_Fail_MaxCount;
		pkt.nIdx = -1;
		pAvatar->Send2Gate( &pkt, true);
		return;
	}

	ParamPool* pPool = pAvatar->GetRoleSet(resPos);
	if(pPool)
	{
		PacketCreateAvatarRes pkt;
		pkt.nFlag = eCreateAvatarRes_Fail;
		pkt.nIdx = -1;
		pAvatar->Send2Gate( &pkt, true);
		return;
	}

	if(pPkt->bCheckData)
	{
		bool bDirtyCheck = DIYDataManager::Instance().CheckDIYDataValid(pPkt);
		if(bDirtyCheck)
		{
			MyLog::error("Bad DIYData!");
			PacketCreateAvatarRes pkt;
			pkt.nFlag = eCreateAvatarRes_Fail;
			pkt.nIdx = -1;
			pAvatar->Send2Gate( &pkt, true);
		}
	}


	pPkt->nAccountLen = uint8(pAvatar->Account.GetAccountName().size());
	if(pPkt->nAccountLen > sizeof(pPkt->account))
	{
		MyLog::error("Bad Account Len = [%d]", pPkt->nAccountLen);
		PacketCreateAvatarRes pkt;
		pkt.nFlag = eCreateAvatarRes_Fail;
		pkt.nIdx = -1;
		pAvatar->Send2Gate( &pkt, true);
		return;
	}

	const std::string strAccount( pPkt->account, pPkt->nAccountLen);

	memcpy( pPkt->account, pAvatar->GetAccountName(), pPkt->nAccountLen);

	PARAM_SET_VALUE( pPool, faceid, pPkt->nFaceID, false);
	PARAM_SET_VALUE( pPool, hairid, pPkt->nHairColor, false);
	PARAM_SET_VALUE( pPool, eyeid, pPkt->nEyeID, false);
	PARAM_SET_VALUE( pPool, skincolor, pPkt->nSkinColor, false);
	PARAM_SET_VALUE( pPool, tattoo, pPkt->nTattoo, false);
	PARAM_SET_VALUE( pPool, beard_eyeshadow, pPkt->nBeard_eyeshadow, false);
	PARAM_SET_VALUE( pPool, morph_1, pPkt->nMorph_1, false);
	PARAM_SET_VALUE( pPool, morph_2, pPkt->nMorph_2, false);
	PARAM_SET_VALUE( pPool, morph_3, pPkt->nMorph_3, false);
	PARAM_SET_VALUE( pPool, fashionid, pPkt->nFashionID, false);
	PARAM_SET_VALUE( pPool, fashioncolor, pPkt->nFashionColor, false);
	PARAM_SET_VALUE( pPool, lipcolor, pPkt->nLipColor, false);

	PARAM_SET_VALUE( pPool, title, strTitle.c_str(), false);
	PARAM_SET_VALUE( pPool, accountname, strAccount.c_str(), false);

	pPkt->nAvatarDID = pAvatar->GenGalaxyUID();

	PARAM_SET_VALUE( pPool, avatardid, pPkt->nAvatarDID, false);
	PARAM_SET_VALUE( pPool, create_time, Time::CurrentTime().Second(), false);

	pPkt->nIdx = resPos;
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

	if(pPkt->nFlag != eCreateAvatarRes_Succ)
	{
		pAvatar->m_account.DestroyRoleSet( pPkt->nIdx);
		pAvatar->Send2Gate( pPkt, true);
		return;
	}

	ParamPool* pPool = pAvatar->GetRoleSet(pPkt->nIdx);
	if(!pPool)
	{
		MyLog::error("there is error in create avatar !!");

		PacketCreateAvatarRes pPkt;
		pPkt.nFlag = eCreateAvatarRes_Fail;
		pPkt.nIdx = -1;
		pAvatar->Send2Gate( &pPkt, true);
		return;
	}

	{
		// tell clt create success
		PacketCreateAvatarRes pkt;
		pkt.nFlag = eCreateAvatarRes_Succ;
		pkt.nIdx = -1;
		pAvatar->Send2Gate( &pkt, true);

		pAvatar->ClearState( eGAS_Base_CltCreateAvatar );
	}

	{
		PacketUserData pkt;

		pkt.nAvatarDID = pPool->GetValue("avatardid", (int64)-1);
		pkt.nIndex = pPkt->nIdx;

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

	MyLog::message("User Req Del Avatar.Account[%s] DelIndex[%d]", pAvatar->GetAccountName(), pPkt->nIdx);
	ParamPool* pPool = pAvatar->GetRoleSet(pPkt->nIdx);
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

	if (pPkt->nFlag == eDelete_Avatar_Succ)
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
		if(!CheckInState(pAvatar, "WorldState_Logined::PktGate_CltSelectAvatar") )
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