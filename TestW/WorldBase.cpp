#include "StdAfx.h"
#include "WorldBase.h"
#include ""

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
			MyLog::performance("[%s] Tick: %dms PlayerOnline: %d", m_strModuleTitle.c_str(), m_nAvgFrameTime, m_olAvatarCount);

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
	WORLDDOG_SET_VALUE( GroupID, Servers.m_nGroupID);

	int32 nSrvID = Servers.MakeServerID( Srv_World, 0);
	SetSrvID( nSrvID );

	m_strDBAIP = "127.0.0.1";
	ConfigManager::GetConfigValue( "CommonConfig", "DBAIP", m_strDBAIP);

	m_nDBAPort = 8888;
	ConfigManager::GetConfigValue( "CommonConfig", "DBAPort", m_nDBAPort);

	WORLDDOG_SET_STRING( DBAIP, m_strDBAIP.c_str());
	WORLDDOG_SET_VALUE( DBAPORT, m_nDBAPort );
	
	WORLDDOG_SET_STRING( MotherIP, ServerConfig::MontherIP.c_str());
	WORLDDOG_SET_VALUE( MotherPort, ServerConfig::MontherPort);

			is there any code?

}

void WorldBase::UpdateDogPool(int32 nFrameTime)
{
			is there any code?
}

void WorldBase::InitDogParamPool(ServerInfo* pInfo)
{
	PeerModuleBase::InitDogParamPool(pInfo);

	Servers.LocalNodeGroup.InitDogDetailsPool( this, pInfo->ServerID);
}

void WorldBase::BroadcastDogPool()
{
	PeerModuleBase::Broadcast2DogPool();
	Servers.LocalNodeGroup.BroadcastDogDetailsPools(this);
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