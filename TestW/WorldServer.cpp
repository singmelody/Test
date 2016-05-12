#include "StdAfx.h"
#include "WorldAvatar.h"
#include "WorldServer.h"

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
