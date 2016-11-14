#include "StdAfx.h"
#include "WorldAvatar.h"
#include "MyLog.h"
#include "WorldAvatarManager.h"
#include "WorldStateManager.h"
#include "AvatarOnLineManager.h"
#include "ServerManager.h"
#include "WorldState.h"
#include "Time.h"
#include "WorldSceneManager.h"
#include "ArenaDefine.h"
#include <sstream>
#include "WorldServer.h"
#include "MyLog.h"
#include "ParamPool.h"
#include "WorldTeamManager.h"

WorldAvatar* GetWorldAvatar(int32 nAvatarID)
{
	return (WorldAvatar*)WorldAvatarManager::Instance().GetAvatar(nAvatarID);
}

WorldAvatar* GetWorldAvatar(PacketBase* pPkt)
{
	if(!pPkt)
		return NULL;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt->GetAvatarID());
	return pAvatar;
}

WorldAvatar* GetWorldAvatarByDID(int64 nAvatarDID)
{
	return AvatarOnLineManager::Instance().GetAvatarByDID(nAvatarDID);
}

WorldAvatar* GetWorldAvatarByNameID(const char* name)
{
	return AvatarOnLineManager::Instance().GetAvatarByName(name);
}


FINISH_FACTORY_ARG0(WorldAvatar);
WorldAvatar::WorldAvatar()
	: m_bLastAvatarInfoValid(false)
	, m_bHasGateAvatar(false)
{
	m_target_scene_point.zero();
	m_target_scene_dir.zero();

	m_pWorld = NULL;

	m_bTargetNodeAvatarCreated = false;
	m_bAvatarDataDirty = false;

	m_curDataOnSide = eCurDataOnDBA;

	InitTickNode(this);

	m_nCurStageID = eWS_Null;
	m_pCurStage = NULL;
	m_bStageChanging = false;

	m_nChangeSceneState = 0;
	m_bIsDestroy = false;

	memset( m_charInterval, 0, sizeof(m_charInterval));
	//memset( &storageData, 0, sizeof(storageData));

	m_PendingCreateSceneID = SCENE_ID_NULL;
	m_nTargetSceneID = SCENE_ID_NULL;

	m_worldArenaState = eMP_STATE_NULL;
	m_WorldAvatarFlag = 0;

	m_nCommonDataMaskFinish = -1;
	m_bTargetNodeAvatarCreated = false;
}


WorldAvatar::~WorldAvatar()
{
	FACTORY_DELOBJ(m_pParamPool);
	RelaseComponent();
}

int64 WorldAvatar::GenUID()
{
	std::stringstream sstr;

	sstr << m_account.GetAccountName() << "." << ServerManager::Instance().m_nZoneID << "." << ServerManager::Instance().m_nGrpID;

	uint32 hashcode = UtilID::CreateFromString( sstr.str().c_str() );
	uint32 second = Time::CurrentTime().Second();

	return uint64(hashcode) << 32 | uint64(second);
}

Scene* WorldAvatar::GetScene()
{
	return WorldSceneManager::Instance().GetScene(m_sceneID);
}

void WorldAvatar::Send2Gate(PacketBase* pPkt, bool bGateProc)
{
	if(!pPkt)
		return;

	pPkt->SetAvatarID(GetAvatarID());
	m_pWorld->Send2Gate( pPkt, m_nGateSrvID, bGateProc);
}

void WorldAvatar::NoticeBillingLogout(bool bExitGame)
{
	if(!WorldServer::bUseBilling)
		return;

	if(!Account.HasBillingOlOnce())
		return;

	if(!m_bLastAvatarInfoValid)
	{
		MyLog::error("WorldAvatar::NoticeBillingLogout Failed LastAvatarInfo Not Valid");
		return;
	}


	if(bExitGame)
	{
		//WorldBillingManager
	}
	else
	{
		if(Account.GetRecentRoleSet())
		{
			m_nLastAvatarLevel = PARAM_GET_VALUE( Account.GetRecentRoleSet(), level, uint8(0));
		}

		PacketNoticeBillingLeaveGame pkt;
		pkt.nAvatarLevel = m_nLastAvatarLevel;
		pkt.bSwitchRole = 1;
		pkt.nUseAccountLen = uint8(Account.GetAccountName().size());

		if( pkt.nUseAccountLen <= sizeof(pkt.m_UserAccount))
		{
			memcpy( pkt.m_UserAccount, Account.GetAccountName().c_str(), pkt.nUseAccountLen);
			Send2Login(&pkt);
		}
		else
		{
			MyLog::error("WorldAvatar::NoticeBillingLogout Bad AccoutLen = [%d]", pkt.nUseAccountLen);
		}
	}
}

void WorldAvatar::SetCurState( WorldStateID newStateID )
{
	if( m_bStageChanging )
	{
		MyLog::error("SetCurStage error, avatarid[%d], curstage[%d], newStage[%d]", GetAvatarID(), m_nCurStageID, newStateID);
		return;
	}

	if( m_nCurStageID == newStateID )
		return;

	m_bStageChanging = true;

	if( m_pCurStage != NULL)
		m_pCurStage->OnLeaveState(this);

	m_nCurStageID = newStateID;
	m_pCurStage = WorldStateManager::Instance().GetState(m_nCurStageID);

	if( m_pCurStage != NULL)
		m_pCurStage->OnEnterState(this);

	m_bStageChanging = false;
}

void WorldAvatar::OnAvatarLeaveGame()
{
	int32 nAvatarID = GetAvatarID();

	// mgr handle offline here


	m_nComState = 0;
}

bool WorldAvatar::Tick(int32 nDelaTime)
{
	TickChatInternal(nDelaTime);
	TickComponent(nDelaTime);

	return false;
}

void WorldAvatar::TickFCM(int32 nDeltaTime)
{
	return;
}

void WorldAvatar::TickChatInternal(int32 nDeltaTime)
{
	for (int32 i = 0; i < CC_MAX; ++i)
	{
		m_charInterval[i] -= nDeltaTime;
		if( m_charInterval[i] < 0)
			m_charInterval[i] = 0;
	}
}

void WorldAvatar::SetChatInterval(int32 nChannel, int32 interval)
{
	if( nChannel < 0 || nChannel >= CC_MAX)
		return;

	m_charInterval[nChannel] = interval;
}

int32 WorldAvatar::GetChatInterval(int32 nChannel)
{
	if( nChannel < 0 || nChannel >= CC_MAX)
		return INT_MAX;

	return m_charInterval[nChannel];
}

void WorldAvatar::RelaseComponent()
{
	//FACTORY_DELOBJ(m_pRelationComponent);
	//FACTORY_DELOBJ(m_pCDComponent);
}

Team* WorldAvatar::GetTeam()
{
	int64 nTeamID = GetTeamID();
	if( nTeamID == INVALID_TEAM_ID )
		return NULL;

	return WorldTeamManager::Instance().GetItem(nTeamID);
}

// make sure the uid is the only one in all srv,
// for the use of merge srv when necessary, especially for 
// the srv zoneid & groupid
int64 WorldAvatar::GenGalaxyUID()
{
	std::stringstream sstr;

	sstr << Account.GetAccountName() <<"."<<ServerManager::Instance().m_nZoneID <<"."<< ServerManager::Instance().m_nGrpID;

	uint32 nHashcode = UtilID::CreateFromString( sstr.str().c_str() );
	uint32 nSecond = Time::CurrentTime().Second();

	return uint64(nHashcode) << 32 | uint64(nSecond);
}

void WorldAvatar::CreateComponent()
{
	
}

void WorldAvatar::ReleaseComponent()
{
	//FACTORY_DELOBJ( m_pRelationCom );

}

bool WorldAvatar::Init(CreateWorldAvatarArg& args)
{
	SetNodeSrvID( SrvID_NULL );
	SetGateSrvID( args.nGateSrvID );
	
	m_nGateSrvID = args.nGateChannelID;

	CreateComponent();

	return true;
}
