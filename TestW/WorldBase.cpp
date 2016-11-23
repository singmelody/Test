#include "StdAfx.h"
#include "WorldBase.h"
#include "WatchDog.h"
#include "MyLog.h"
#include "ConfigManager.h"
#include "ParamPool.h"
#include "WorldSceneManager.h"
#include "ServerConfig.h"
#include "WorldAvatarManager.h"


WorldBase::WorldBase(void)
	: PeerModuleBase( eSrv_World )
{
	m_olAvatarCount = 0;
	m_bWarWorld = false;
	m_pWorldDogPool = NULL;
}


WorldBase::~WorldBase(void)
{
}


bool WorldBase::Init(int32 nArgc, char* argv[])
{
	if(!PeerModuleBase::Init( nArgc, argv))
		return false;

	return true;
}

void WorldBase::ProcessConsole(int32 nFrameTime)
{
	if( SERVER_TICK_TIME_SHOW > 0 )
	{
		static uint32 nTickShowTime = 0;
		nTickShowTime += nFrameTime;

		if( nTickShowTime >= SERVER_TICK_TIME_SHOW)
		{
			nTickShowTime = 0;

			CalculateStatistics();
			ResetStatistics();

			WatchDog::Instance().NextStep(GAME_THREAD_WATCHDOG_ID);
			MyLog::message("[%s] Tick: %dms PlayerOnline: %d", m_strModueTitle.c_str(), m_nAvgFrameTime, m_olAvatarCount);

		}
	}
}

bool WorldBase::CreateDogPool()
{
	if( !PeerModuleBase::CreateDogPool() )
		return false;

	m_pWorldDogPool = ParamSet::CreateNew( eParam_WorldInfo, 0);
	if(!m_pWorldDogPool)
		return false;

	m_listDogData.push_back( m_pWorldDogPool );

	return true;
}

void WorldBase::OnConfigLoaded()
{
	PeerModuleBase::OnConfigLoaded();

	Servers.FillConfig();

	WORLDDOG_SET_VALUE( ZoneID, Servers.m_nZoneID);
	WORLDDOG_SET_VALUE( GroupID, Servers.m_nGrpID);


	m_strDBAIP = "127.0.0.1";
	ConfigManager::GetConfigValue( "CommonConfig", "DBAIP", m_strDBAIP);

	m_nDBAPort = 8888;
	ConfigManager::GetConfigValue( "CommonConfig", "DBAPort", m_nDBAPort);

	WORLDDOG_SET_STRING( DBAIP, m_strDBAIP.c_str());
	WORLDDOG_SET_VALUE( DBAPort, m_nDBAPort );
	
	WORLDDOG_SET_STRING( MotherIP, ServerConfig::sMotherIP.c_str());
	WORLDDOG_SET_VALUE( MotherPort, ServerConfig::nMotherPort);
}

void WorldBase::UpdateDogPool(int32 nFrameTime)
{
	PeerModuleBase::UpdateDogPool(nFrameTime);

	WORLDDOG_SET_VALUE( PlayerCnt, WorldAvatarManager::Instance().size());

	{
		WorldSceneManager& mgr = WorldSceneManager::Instance();

		mgr.SumSceneCount();

		WORLDDOG_SET_VALUE( SceneLoadValue, mgr.m_nSceneLoadValue);
		WORLDDOG_SET_VALUE( SceneCnt, mgr.m_nSceneCnt);
		WORLDDOG_SET_VALUE( TrunkCnt, mgr.m_nTrunkCnt);
		WORLDDOG_SET_VALUE( CopyCnt, mgr.m_nCopyCnt);
		WORLDDOG_SET_VALUE( TeamCopyCnt, mgr.m_nTeamCopyCnt);
		WORLDDOG_SET_VALUE( GuildCopyCnt, mgr.m_nGuildCopyCnt);
		WORLDDOG_SET_VALUE( TrunkCopyCnt, mgr.m_nTrunkCopyCnt);
		WORLDDOG_SET_VALUE( RootCopyCnt, mgr.m_nRootCopyCnt);

	}

	{
// 		WORLDDOG_SET_VALUE( WS_Logining, WorldState_Logining::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_Logined, WorldState_Logined::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_DataLoading, WorldState_DataLoading::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_DataReady, WorldState_DataReady::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_WaitScene, WorldState_WaitScene::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_Decommission, WorldState_Decommission::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_EnterGame, WorldState_EnterGame::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_Jumping, WorldState_Jumping::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_Gaming, WorldState_Gaming::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_ExitGame, WorldState_ExitGame::Instance().m_listAvatars.GetCount() );
// 		WORLDDOG_SET_VALUE( WS_Billing, WorldState_Billing::Instance().m_listAvatars.GetCount() );
	}

	Servers.m_LocalNodeGrp.UpdateDogDetailsPools();
}

void WorldBase::InitDogParamPool(ServerInfo* pInfo)
{
	PeerModuleBase::InitDogParamPool(pInfo);

	Servers.m_LocalNodeGrp.InitDogDetailsPools( this, pInfo->nSrvID);
}

void WorldBase::BroadcastDogPool()
{
	PeerModuleBase::Broadcast2DogPool();
	Servers.m_LocalNodeGrp.BroadcastDogDetailsPools(this);
}

void WorldBase::OnAddServerInfo(ServerInfo* pInfo)
{
	PeerModuleBase::OnAddServerInfo(pInfo);
	OnServerInfoChanged(pInfo);
}

void WorldBase::OnRemoveServerInfo(ServerInfo* pInfo)
{
	PeerModuleBase::OnRemoveServerInfo(pInfo);
	OnServerInfoChanged(pInfo);
}

void WorldBase::OnServerInfoChanged(ServerInfo* pInfo)
{
	switch(pInfo->nSrvType)
	{
	case eSrv_DBA:
		WORLDDOG_SET_VALUE( DBAID, pInfo->nSrvID);
		break;

	case eSrv_Login:
		WORLDDOG_SET_VALUE( LoginID, pInfo->nSrvID);
		break;

	case eSrv_Gate:
		WORLDDOG_SET_VALUE( GateCnt, Servers.m_GateGrp.ServerCnt());
		break;

	case eSrv_Node:
		WORLDDOG_SET_VALUE( LocalNodeCnt, Servers.m_LocalNodeGrp.ServerCnt() );
		WORLDDOG_SET_VALUE( RemoteNodeCnt, Servers.m_RemoteNodeGrp.ServerCnt() );
		break;

	case eSrv_Collision:
		break;

	case eSrv_World:
		{
			if(m_bWarWorld)
			{
				WORLDDOG_SET_VALUE( SubWorldCnt, Servers.m_RemoteWorldGrp.ServerCnt());
			}
			else
			{
				WORLDDOG_SET_VALUE( WarID, pInfo->nSrvID);
			}
		}
 		break;
	}
}