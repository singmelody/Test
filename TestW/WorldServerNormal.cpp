#include "StdAfx.h"
#include "WorldServerNormal.h"
#include "ConfigManager.h"
#include "ParamPool.h"
#include "MyLog.h"
#include "WorldSceneManager.h"
#include "WorldGuildManager.h"

WorldServerNormal::WorldServerNormal(void)
{
	m_nGateCount = 1;
	m_bWarWorld  = false;

	m_nConnectWarState = eCWS_Waiting;
	m_nConnectWarInterval = 5000;

	m_bUseConnectionThread = true;
}


WorldServerNormal::~WorldServerNormal(void)
{
}

bool WorldServerNormal::Init(int32 nArgc, char* argv[])
{
	if(!WorldServer::Init( nArgc, argv))
		return false;

	return true;
}

void WorldServerNormal::ProcessLogic(int32 nFrameTime)
{

}

void WorldServerNormal::RegPeerPktHandle(PacketProcessor* pProc)
{

}

void WorldServerNormal::PacketWorldWarInit(class PacketWorldWarInit* pPkt)
{

}

void WorldServerNormal::OnConfigLoaded()
{
	WorldServer::OnConfigLoaded();

	ConfigManager::GetConfigValue( "WorldConfig/Cluster", "GateCount", m_nGateCount);

	ConfigManager::GetConfigValue("CommonConfig", "WarIP", m_WarIP);
	ConfigManager::GetConfigValue("CommonConfig", "WarPort", m_WarPort);

	WORLDDOG_SET_VALUE( WarWorld, 0);

	if( Servers.m_nGrpID <= 0 || Servers.m_nGrpID > eSrvID_MaxGrpID)
	{
		MyLog::error("WorldServerNormal::OnConfigLoaded() Bad SrvGrpID=[%d]", Servers.m_nGrpID);
		return;
	}

	int32 nSrvID = Servers.MakeSrvID( eSrv_World, 0);
	SetSrvID(nSrvID);
}

void WorldServerNormal::AppendLoadTemplate()
{
	WorldServer::AppendLoadTemplate();


}

bool WorldServerNormal::ClusterCheck()
{
	if(!WorldServer::ClusterCheck())
		return false;

	int32 nCurGateCnt = Servers.m_GateGrp.ServerCnt();

	if( nCurGateCnt < m_nGateCount )
	{
		MyLog::message("Gate Check CurCount[%d] NeedCount[%d]", nCurGateCnt, m_nGateCount);
		return false;
	}

	if(Servers.GetLoginInfo() == NULL)
	{
		MyLog::error("Login Check Failed");
		return false;
	}

	return true;
}

void WorldServerNormal::OnWorldConnect(int32 nSrvID, int32 nSocketID, bool bWarGrp, SockAddr& laddr)
{
	if( bWarGrp )
	{
		AUTOLOCK(m_lockLoginWar);

		ServerInfo* pInfo = Servers.AddWarWorld( nSrvID, nSocketID, laddr);
		if( pInfo != NULL )
		{
			m_nConnectWarState = eCWS_Succeed;
			SetModuleName("SubWorld");

			WorldGuildManager::Instance().OnConnectWarWorld();
		}
	}
	else
	{
		MyLog::error("World connect to World Error !");
		return;
	}

}

void WorldServerNormal::OnWorldDisconnect(ServerInfo* pInfo)
{
	MyLog::error("<<--Server Manager Remove War World!");

	AUTOLOCK(m_lockLoginWar);

	m_nConnectWarState = eCWS_Waiting;
	Servers.m_pWarWorld = NULL;

	SetModuleName("World");

	WorldGuildManager::Instance().OnDisconnectWarWorld();
}

void WorldServerNormal::OnRecvSrvInfoPkt(PacketAddSrvInfo* pPkt)
{
	WorldServer::OnRecvSrvInfoPkt(pPkt);
}

void WorldServerNormal::OnAddLoginInfo(ServerInfo* pInfo)
{
	WorldServer::OnAddLoginInfo(pInfo);

	BroadcastServerInfo2Gate(pInfo);
}

void WorldServerNormal::OnAddNodeInfo(ServerInfo* pInfo)
{
	WorldServer::OnAddNodeInfo(pInfo);

	//BroadcastServerInfo2Gates( pInfo );

	WorldSceneManager::Instance().SyncParallelInfo2Node( pInfo->nSrvID );


}

void WorldServerNormal::OnAddGateInfo(ServerInfo* pInfo)
{
	WorldServer::OnAddGateInfo(pInfo);

	SendServerInfos( pInfo->nSocketID, eSM_Login | eSM_LocalNode | eSM_RemoteNode);
}
