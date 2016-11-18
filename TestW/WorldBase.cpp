#include "StdAfx.h"
#include "WorldBase.h"
#include "WatchDog.h"
#include "MyLog.h"
#include "ConfigManager.h"
#include "ParamPool.h"
#include "WorldSceneManager.h"

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
	WORLDDOG_SET_VALUE( DBAPORT, m_nDBAPort );
	
	WORLDDOG_SET_STRING( MotherIP, ServerConfig::MontherIP.c_str());
	WORLDDOG_SET_VALUE( MotherPort, ServerConfig::MontherPort);
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
}

void WorldBase::InitDogParamPool(ServerInfo* pInfo)
{
	PeerModuleBase::InitDogParamPool(pInfo);

	Servers.m_LocalNodeGrp.InitDogDetailsPool( this, pInfo->nSrvID);
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
		WORLDDOG_SET_VALUE( GateCnt, Servers.GateGroup.ServerCnt());
		break;

	case eSrv_Node:
		WORLDDOG_SET_VALUE( LocalNodeCnt, Servers.LocalNodeGroup.ServerCnt() );
		WORLDDOG_SET_VALUE( RemoteNodeCnt, Servers.RemoteNodeGroup.ServerCnt() );
		break;

	case eSrv_Collision;
		break;
		
		is there any code?

	}
}