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
#include "WorldEnterManager.h"
#include "PacketImpl.h"
#include "ParamTypeDef.h"
#include "WorldAvatarCommonData.h"
#include "CommonDataObj.h"
#include "WorldLoginActionManager.h"
#include "ParamPool.h"

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
	m_pCurState = NULL;
	m_bStateChanging = false;

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

	m_pWorld = NULL;

	m_bStorageDataOnNode = false;
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
	return WorldSceneManager::Instance().GetScene(m_nSceneID);
}

void WorldAvatar::SendPacket(PacketBase* pkt)
{
	if(!pkt || !m_pWorld)
		return;

	pkt->SetAvatarID( GetAvatarID());
	m_pWorld->Send2Gate( pkt, m_nGateSrvID);
}

void WorldAvatar::Send2DBA(PacketBase* pPkt)
{
	if(!pPkt)
		return;

	pPkt->SetAvatarID(GetAvatarID());
	m_pWorld->Instance().Send2DBA(pPkt);
}

void WorldAvatar::Send2Node(PacketBase* pPkt, int32 nSrvID)
{
	if(!pPkt)
		return;

	pPkt->SetAvatarID(GetAvatarID());
	m_pWorld->Instance().Send2Node( pPkt, nSrvID);
}

void WorldAvatar::Send2Login(PacketBase* pPkt)
{
	if(!pPkt)
		return;

	pPkt->SetAvatarID(GetAvatarID());
	m_pWorld->Instance().Send2Login( pPkt);
}

void WorldAvatar::Send2Gate(PacketBase* pPkt, bool bGateProc)
{
	if(!pPkt)
		return;

	pPkt->SetAvatarID(GetAvatarID());
	m_pWorld->Send2Gate( pPkt, m_nGateSrvID, bGateProc);
}

void WorldAvatar::Send2CurNode(PacketBase& pkt)
{
	Send2Node( &pkt, GetNodeSrvID());
}

int32 WorldAvatar::GetAvatarNameID()
{
	if(!m_pParamPool)
		return -1;

	const char* pUserName = m_pParamPool->GetValueString("title", NULL);
	return UtilID::CreateFromString(pUserName);
}

void WorldAvatar::SetParamPool(ParamPool* p)
{
	if(!p)
		return;

	AvatarSrv::SetParamPool(p);
}

class WorldAvatarCommonData* WorldAvatar::GetCommonDataManager() const
{
	return static_cast<WorldAvatarCommonData*>(m_pCommonDataMgr);
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
	if( m_bStateChanging )
	{
		MyLog::error("SetCurStage error, avatarid[%d], curstage[%d], newStage[%d]", GetAvatarID(), m_nCurStageID, newStateID);
		return;
	}

	if( m_nCurStageID == newStateID )
		return;

	m_bStateChanging = true;

	if( m_pCurState != NULL)
		m_pCurState->OnLeaveState(this);

	m_nCurStageID = newStateID;
	m_pCurState = WorldStateManager::Instance().GetState(m_nCurStageID);

	if( m_pCurState != NULL)
		m_pCurState->OnEnterState(this);

	m_bStateChanging = false;
}


bool WorldAvatar::PushAllData2CurNode()
{
	int32 nNodeSrvID = GetNodeSrvID();

	ServerInfo* pInfo = ServerManager::Instance().GetNodeInfo( nNodeSrvID );
	if(!pInfo)
		return false;

	int32 nAvatarID = GetAvatarID();

	{
		ParamPool* pPool = (ParamPool*)GetParamPool();
		if(!pPool)
		{
			PacketAvatarData pkt;

			pkt.nSrcAvatarID = nAvatarID;
			pkt.nParamType = GetParamTypeID();

			pkt.SyncParam2Node( this, nNodeSrvID, pPool, eParam_Flag_Server, eParam_Sync_All);
		}
	}

	WorldAvatarCommonData* pData = GetCommonDataManager();
	if(pData)
		pData->InitData2Node(nNodeSrvID);

	if(GetTeamID() != INVALID_TEAM_ID )
	{
		WorldTeam* pTeam = GetWorldTeam();
		if(pTeam && pTeam->GetItem(GetAvatarDID()) != NULL)
		{
			// send complete team struct
		}
		else
		{
			SetTeamID(INVALID_TEAM_ID);

			// update info 2 node
		}
	}
	else
	{
		
	}

	WorldLoginActionManager::Instance().SendAction2Node(this);
	return true;
}

void WorldAvatar::HandleCreateSceneResult(int32 nResult, WorldScene* pScene /*= NULL*/)
{
	if(GetScene() == NULL)
	{
		SetPendingCreateScene(SCENE_ID_NULL);

		if( nResult == eCreateScene_Succeed)
			WorldEnterManager::Instance().HandleEnterScene( this, pScene);
		else
			OnEnterGameFailed();
	}
	else
	{
		if( nResult != eCreateScene_Succeed )
			SetPendingCreateScene(SCENE_ID_NULL);

		WorldSceneManager::Instance().NotifyCreateSceneResult( GetNodeSrvID(), GetAvatarID(), nResult, pScene);
		SetCurState(eWS_Gaming);
	}
}

void WorldAvatar::OnEnterGameFailed()
{
	SetCurState(eWS_ExitGame);
}

void WorldAvatar::OnAvatarEnterGame()
{
	int32 nAvatarID = GetAvatarID();
	MyLog::message("Avatar Req EnterGame Succ, AvatarID[%d] Account[%s]", nAvatarID, GetAccountName());

}

void WorldAvatar::OnAvatarLeaveGame()
{
	int32 nAvatarID = GetAvatarID();

	// mgr handle offline here


	m_nComState = 0;
}

bool WorldAvatar::CheckPlayerEnterScene(WorldScene* pScene)
{
	// judge the condition
	// 1.change to arena scene, stop the thing that disturb this action
	// 2.when loading map, distub to enter arena scene


	return true;
}

void WorldAvatar::SyncAvatarData2DB(bool bExitGame, int32 nSyncFlag)
{
	
	ParamPool* pPool = GetParamPool();
	if(!pPool)
	{
		PacketAvatarData2DBA pkt;
		pkt.nParamType = GetParamTypeID();
		pkt.nSrcAvatarID = GetAvatarID();
		pkt.nAvatarDID = GetAvatarDID();
		pkt.nFlag = eAvatarData_Destroy;

		pkt.SyncParamPool( this, &AvatarSrv::Send2DBA, pPool, eParam_Flag_Save, nSyncFlag);
	}

	int32 nAvatarID = GetAvatarID();
	WorldAvatarCommonData* pMgr = GetCommonDataManager();
	if( pMgr != NULL )
	{
		PacketPack pack;
		pack.SetDBASyncArg(this);

		for (int32 i = 0; i < eCommonData_Max; ++i)
		{
			CommonDataCont* pCont = pMgr->GetCont(CommonDataType(i));
			if(pCont)
			{
				for (int32 j = 0; j < pCont->GetMaxSize(); ++j)
				{
					CommonDataObject* pObj = pCont->Get(j);
					if(!pObj)
						continue;

					if(pObj->NeedSync2DBA())
					{
						PacketCommonDataUpdate pkt;
						pkt.SetAvatarID( nAvatarID );
						pObj->InitPacket( pkt, this);

						pkt.SyncParamPool( &pack, pObj->GetParamPool(), eParam_Flag_Save, nSyncFlag);
					}
				}
			}
		}

		pack.SendPacket();
	}


	{
		PacketAvatarDataSend2DBAFin pkt;

		pkt.SetAvatarID(GetAvatarID());
		pkt.nMode = bExitGame ? PacketAvatarDataSend2DBAFin::eExitGame : PacketAvatarDataSend2DBAFin::eInterbackup;
		pkt.nAvatarDID = GetAvatarDID();
		Send2DBA(&pkt);
	}
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

void WorldAvatar::TickComponent(int32 nDeltaTime)
{
// 	if(m_pRelationCom)
// 		m_pRelationCom->WorldTick( *this, nDeltaTime);
// 
// 	if(m_pCDCom)
// 		m_pCDCom->WorldTick(nDeltaTime);
}

bool WorldAvatar::RequestBillingLogin()
{
// 	if(!WorldServer::bUseBilling)
// 		return false;
// 
// 	{
// 		m_nLastAvatarLevel = uint8(GetLevel());
// 		m_nLastAvatarDID = GetAvatarDID();
// 
// 		const char* pSzAvatar = GetParamPool()->GetValueString( "title", NULL);
// 		if(!pSzAvatar)
// 		{
// 			MyLog::error("WorldAvatar::RequestBillingLogin Failed to get pSzAvatar");
// 			return false;
// 		}
// 
// 		m_strLastAvatarName = std::string(pSzAvatar);
// 		m_bLastAvatarInfoValid = true;
// 	}
// 
// 	if(!m_bLastAvatarInfoValid)
// 	{
// 		MyLog::error("WorldAvatar::RequestBillingLogin Failed LastAvatarInfoValid Not Valid");
// 		return false;
// 	}

	return false;
}

void WorldAvatar::RelaseComponent()
{
	//FACTORY_DELOBJ(m_pRelationComponent);
	//FACTORY_DELOBJ(m_pCDComponent);
}

void WorldAvatar::NotifyCltKickout(int8 nReason)
{
	MyLog::message("<KickAvatar> reason=[%d] accountname=[%s] avatardid=[%lld]", nReason, GetAccountName(), GetAvatarDID());

	PacketKickOutNotifyClt notifyPkt;
	notifyPkt.nReason = nReason;
	SendPacket(&notifyPkt);
}

void WorldAvatar::SyncAvatarStorageItemData2DB()
{
	// sync item data
	return;
}

void WorldAvatar::OnAfterPullDataFromNode()
{
	WorldAvatarGroup::OnAfterPullDataFromNode();
	if(m_bStorageDataOnNode)
	{
		SyncAvatarStorageItemData2DB();
		m_bStorageDataOnNode = false;
	}
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
	WorldAvatarCommonData* pCommonDataMgr = FACTORY_NEWOBJ(WorldAvatarCommonData);
	if( pCommonDataMgr )
	{
		
	}
}

void WorldAvatar::ReleaseComponent()
{
	//FACTORY_DELOBJ( m_pRelationCom );

}

void WorldAvatar::ClearParamPool()
{
	m_pParamPool = NULL;
}

bool WorldAvatar::Init(CreateWorldAvatarArg& args)
{
	SetNodeSrvID( SrvID_NULL );
	SetGateSrvID( args.nGateSrvID );
	
	m_nGateSrvID = args.nGateChannelID;

	CreateComponent();

	return true;
}

bool IsStorageItem(ParamPool& pool)
{
	assert( pool.GetParamDefineIndex() == eParam_Item);

	bool b = (PARAM_GET_VALUE( (&pool), bagtype, (int8)0) == 3);

	return b;
}