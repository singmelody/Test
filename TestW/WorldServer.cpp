#include "StdAfx.h"
#include "WorldServer.h"

#include "WorldAvatar.h"
#include "WorldAvatarManager.h"
#include "ConfigManager.h"
#include "MyLog.h"
#include "LoadTemplateManager.h"
#include "AvatarOnLineManager.h"
#include "WorldSceneManager.h"
#include "WorldAvatar.h"
#include "WorldState.h"
#include "WatchDog.h"

bool WorldServer::bUseBilling = false;

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


	return true;
}

bool WorldServer::Start()
{
	WatchDog::Instance().RegWatchDog( GAME_THREAD_WATCHDOG_ID, "World-Game-Thread", WORLD_FRAME_TIME*10);
	return StartMainLoop( WORLD_FRAME_TIME);
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


void WorldServer::KickOutAvatar(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return;

	if(pAvatar->GetType() != eType_Player)
		return;

	if(IsSrvShutingdown())
		return;

	pAvatar->NotifyCltKickout(PacketKickOutNotifyClt::eReason_GM);

	pAvatar->GetCurState()->DestroyAvatar(pAvatar);
}

bool WorldServer::ClusterCheck()
{
	int32 nCurNodeCnt = Servers.m_LocalNodeGrp.ServerCnt();

	if( nCurNodeCnt < m_nNodeCount )
	{
		MyLog::message("WorldServer::ClusterCheck() Node Check CurCount[%d] NeedCnt[%d]", nCurNodeCnt, m_nNodeCount);
		return false;
	}

	int32 nCurCollisionCnt = Servers.m_CollisionGroup.ServerCnt();
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

void WorldServer::ProcessLogic(int32 nFrameTime)
{
	WorldBase::ProcessLogic( nFrameTime );

	WorldSceneManager::Instance().Tick( nFrameTime );
}

void WorldServer::OnConfigLoaded()
{
	WorldBase::OnConfigLoaded();

	WorldAvatarManager::Instance().SetSrvGroupID( Servers.m_nGrpID );

	ConfigManager::GetConfigValue("WorldConfig/Cluster", "NodeCount", m_nNodeCount);
	ConfigManager::GetConfigValue("WorldConfig/Cluster", "CollisionCount", m_nCollisionCount);

	AvatarOnLineManager::Instance().OnConfigLoaded();

	SockAddr laddr( m_PeerIP, m_PeerPort);
	Servers.AddLocalWorld( m_nSrvID, -1, laddr);
}

void WorldServer::AppendLoadTemplate()
{
	WorldBase::AppendLoadTemplate();

	Templates.AddTemplate("SceneManager", WorldSceneManager::InstancePtr());
}

void WorldServer::SubWorld2WarWorld(class PacketBase* pPkt)
{
	ServerInfo* pWarInfo = Servers.GetWarWorldInfo();
	if(pWarInfo)
		PeerSend( pPkt, pWarInfo->nSocketID);
}

bool WorldServer::PreProcessShutdown(int32 nArgc, char* argv[])
{
	return true;
}

void WorldServer::SendServerInfos(int32 nSocketID, int32 nMask)
{
	if( nMask & eSM_World )
		SendServerInfo( nSocketID, Servers.GetLocalWorldInfo());

	if( nMask & eSM_Login )
		SendServerInfo( nSocketID, Servers.GetLoginInfo() );

	if( nMask & eSM_LocalNode)
		Servers.m_LocalNodeGrp.SyncGroupInfo(nSocketID);

	if( nMask &	eSM_RemoteNode)	
		Servers.m_RemoteNodeGrp.SyncGroupInfo(nSocketID);

	if( nMask &	eSM_Gate)
		Servers.m_GateGrp.SyncGroupInfo(nSocketID);
}

void WorldServer::SendServerInfo(int32 nSocketID, ServerInfo* pInfo)
{
	if( pInfo == NULL)
		return;

	PacketAddSrvInfo pkt;
	pInfo->FillPacket(&pkt);

	PeerSend( &pkt, nSocketID);
}
