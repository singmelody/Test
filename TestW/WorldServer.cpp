#include "StdAfx.h"
#include "WorldAvatar.h"
#include "WorldServer.h"
#include "WorldAvatarManager.h"

WorldServer::WorldServer()
	: m_shutdownStage(eSDS_None)
	, m_bBillingRested(false)
{
	m_nNodeCount = 1;
	m_nCollisionCount = 1;
}

WorldServer::~WorldServer()
{

}

bool WorldServer::Init(int32 nArgc, char* argv[])
{
	if(!WorldBase::Init( nArgc, argv))
		return false;

	MyLog::message("Begin LoadMoule[WorldSrv]");
	Templates.Load("WorldServer");
	MyLog::message("End LoadMoule[WorldSrv]");
}

void WorldServer::OnGateDisconnect(ServerInfo* pInfo)
{
	WorldStateManager::Instance().OnNodeCrashed( pInfo->nSrvID, pInfo->m_bUseSHM);
	WorldSceneManager::Instance().OnNodeCrashed( pInfo->nSrvID );
}

void WorldServer::OnNodeDisconnect(ServerInfo* pInfo)
{

}

void WorldServer::OnRecvSrvConnectPkt(class PacketSrvConnect* pPkt)
{

}

void WorldServer::OnAllocateSrvID(int32 nSrvType, int32 nSrvID, int32 nSocketID)
{

}

bool WorldServer::ClusterCheck()
{
	int32 nCurNodeCnt = Servers.m_LocalNodeGrp.ServerCnt();

	if( nCurNodeCnt < m_nNodeCount )
	{
		MyLog::message("WorldServer::ClusterCheck() Node Check CurCount[%d] NeedCnt[%d]", nCurNodeCnt, m_nNodeCount);
		return false;
	}

	int32 nCurCollisionCnt = Servers.CollisionGroup.ServerCnt();
	if( nCurCollisionCnt < m_nCollisionCount)
	{
		MyLog::message("WorldServer::ClusterCheck() Node Check CurCount[%d] NeedCnt[%d]", nCurCollisionCnt, m_nCollisionCount);
		return false;
	}

	if(!Servers.GetDBAInfo())
	{
		MyLog::message("WorldServer:;ClusterCheck() DBA Check Fail");
		return false;
	}

	return true;
}


void WorldServer::OnConfigLoaded()
{
	WorldBase::OnConfigLoaded();

	WorldAvatarManager::Instance().SetServerGroupID( Servers.m_nGrpID );

	ConfigManager::GetConfigValue("WorldConfig/Cluster", "NodeCount", m_NodeCnt);
	ConfigManager::GetConfigValue("WorldConfig/Cluster", "CollisionCount", m_nCollisionCount);

	AvatarOnLineManager::Instance().OnConfigLoaded();

	SockAddr laddr( m_peerIP, m_peerPort);
	Servers.AddLocalWorld( m_nSrvID, -1, laddr);
}