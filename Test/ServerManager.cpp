#include "StdAfx.h"
#include "ServerManager.h"
#include "MyLog.h"
#include "PeerModuleBase.h"

const char* GetSrvTitle(int32 nSrvType)
{
	if( nSrvType < 0 || nSrvType >= eSrv_Count )
		nSrvType = 0;

	return SrvTitle[nSrvType];
}

ServerManager::ServerManager(void)
{
	m_nZoneID = 0;
	m_nGrpID = 0;

	m_pLocalWorld = NULL;
	m_localWorldChannelID = -1;

	m_pWarWorld = NULL;

	m_pLogin = NULL;
	m_pDBA = NULL;

	m_srvType = eSrv_Unkown;

	m_LocalNodeGrp.Init( this, eSrv_Node);
	m_LocalNodeGrp.m_strSrvName = "LocalNode";
	
	m_RemoteNodeGrp.Init( this, eSrv_Node);
	m_RemoteNodeGrp.m_strSrvName = "RemoteNode";

	m_GateGrp.Init( this, eSrv_Gate);

	m_RemoteWorldGrp.Init( this, eSrv_World);
	m_NodeDataSyncGrp.Init( this, eSrv_NodeSHM);
}


ServerManager::~ServerManager(void)
{
}

int32 ServerManager::GetZoneID(int32 nSrvID)
{
	return ( nSrvID >> eSrvID_ZoneStartBit ) & eSrvID_MaxZoneID;
}

int32 ServerManager::GetGroupID(int32 nSrvID)
{
	return ( nSrvID >> eSrvID_GrpStartBit) & eSrvID_MaxGrpID;
}

int32 ServerManager::GetSrvType(int32 nSrvID)
{
	return ( nSrvID >> eSrvID_TypeBits) & eSrvID_MaxTypeID;
}

int32 ServerManager::GetSrvIndex(int32 nSrvID)
{
	return nSrvID & eSrvID_MaxIndexBits;
}

void ServerManager::SetPeerModule(PeerModuleBase* pPeerModule)
{
	m_pPeerModule = pPeerModule;
}

ServerInfo* ServerManager::GetLoginInfo()
{
	return m_pLogin;
}

ServerInfo* ServerManager::GetDBAInfo()
{
	 return m_pDBA;
}


ServerInfo* ServerManager::GetGateInfo(int32 nID)
{
	return m_GateGrp.GetServer(nID);
}

ServerInfo* ServerManager::GetWarWorldInfo()
{
	return m_pWarWorld;
}

void ServerManager::SetSrvType(SrvType nSrvType)
{
	m_srvType = nSrvType;
}

// zoneid	groupid		type	index
//	31-24	23 - 18	   17-13	
//
int32 ServerManager::MakeSrvID(int32 nZoneID, int32 nGrpID, int32 nSrvType, int32 nIdx)
{
	int32 nSrvID = ( nZoneID << eSrvID_ZoneStartBit) + ( nGrpID << eSrvID_GrpStartBit) 
		+ ( nSrvType << eSrvID_TypeStartBit) + nIdx;

	return nSrvID;
}

int32 ServerManager::MakeSrvID(int32 nSrvType, int32 nSrvIdx)
{
	return MakeSrvID( m_nZoneID, m_nGrpID, nSrvType, nSrvIdx);
}

bool ServerManager::AddLocalWorld(int32 nSrvID, int32 nSocketID, SockAddr& addr)
{
	if(!m_pLocalWorld)
		return false;

	MyLog::message("-->>Srv Manager Registry World SocketID=%d\n", nSocketID);

	ServerInfo* pInfo = new ServerInfo( eSrv_World, nSrvID, nSocketID, addr);
	if(!pInfo)
		return false;

	m_pLocalWorld = pInfo;
	m_localWorldChannelID = nSocketID;

	AddSrvInfo(pInfo);
	return true;
}

ServerInfo* ServerManager::AddDBA(int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	if(m_pDBA != NULL)
		return NULL;

	MyLog::message("->>Srv Manager Registry DBA iSocketID = %d\n", nSocketID);

	ServerInfo* pInfo = new ServerInfo( eSrv_DBA, nSrvID, nSocketID, laddr);
	if(!pInfo)
		return NULL;

	m_pDBA = pInfo;
	AddSrvInfo(pInfo);
	return m_pDBA;
}

ServerInfo* ServerManager::AddLogin(int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	if(!m_pLogin)
		return NULL;

	MyLog::message("->>Srv Manager Registry Login iSocketID = %d\n", nSocketID);

	ServerInfo* pInfo = new ServerInfo( eSrv_Login, nSrvID, nSocketID, laddr);
	if(!pInfo)
		return NULL;

	m_pLogin = pInfo;
	AddSrvInfo(pInfo);
	return m_pLogin;
}

ServerInfo* ServerManager::AddNode(int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	ServerInfo* pInfo = NULL;
	if(IsSameGroup( nSrvID ))
		pInfo = m_LocalNodeGrp.AddServer( nSrvID, nSocketID, laddr);
	else
		pInfo = m_RemoteNodeGrp.AddServer( nSrvID, nSocketID, laddr);

	return pInfo;
}

bool ServerManager::IsSameGroup(int32 nSrvID)
{
	int32 nGrpID = GetGroupID(nSrvID);
	return nGrpID == m_nGrpID;
}

ServerInfo* ServerManager::AddSrvInfo(SrvItem* pItem)
{
	int32 nSocketID = pItem->nSocketID;
	int32 nSrvID = pItem->nSrvID;

	SockAddr laddr( pItem->listenIpPeer, pItem->listenPortPeer);

	switch( pItem->nSrvType )
	{
	case eSrv_DBA:
		AddDBA( nSrvID, nSocketID, laddr);
		break;

	case eSrv_World:
		if(IsSameGroup( nSrvID ))
			AddLocalWorld( nSrvID, nSocketID, laddr);
		else
			m_RemoteWorldGrp.AddServer( nSrvID, nSocketID, laddr);

	case eSrv_Login:
		AddLogin( nSrvID, nSocketID, laddr);
		break;

	case eSrv_Node:
		AddNode( nSrvID, nSocketID, laddr);
		break;

	case eSrv_Gate:
		{
			GateSrvInfo* pInfo = (GateSrvInfo*)m_GateGrp.AddServer( nSrvID, nSocketID, laddr);
			if(pInfo)
				pInfo->SetGateSrvInfo( pItem->listenPortClt, pItem->listenIpClt);
		}
		break;

	case eSrv_Collision:
		break;

	case eSrv_DOG:
		m_DogGroup.AddServer( nSrvID, nSocketID, laddr);
		break;
	}

	ServerInfo* pInfo = GetSrvBySrvID(nSrvID);
	return pInfo;
}

void ServerManager::AddSrvInfo(ServerInfo* pInfo)
{
	if(!pInfo)
		return;

	m_SrvBySrvID[pInfo->nSrvID] = pInfo;
	m_SrvBySocketID[pInfo->nSocketID] = pInfo;
}


void ServerManager::RemoveSrvInfo(ServerInfo* pInfo)
{
	if(!pInfo)
		return;

	SrvInfoMap::iterator itr = m_SrvBySrvID.find( pInfo->nSrvID );
	if( itr != m_SrvBySrvID.end() )
		m_SrvBySrvID.erase(itr);

	SrvInfoMap::iterator itr1 = m_SrvBySocketID.find( pInfo->nSocketID );
	if( itr1 != m_SrvBySocketID.end() )
		m_SrvBySocketID.erase(itr1);
}

ServerInfo* ServerManager::GetSrvBySocketID(int32 nSocketID)
{
	SrvInfoMap::iterator itr = m_SrvBySocketID.find( nSocketID );
	if( itr != m_SrvBySocketID.end() )
		return itr->second;

	return NULL;
}

ServerInfo* ServerManager::GetSrvBySrvID(int32 nSrvID)
{
	SrvInfoMap::iterator itr = m_SrvBySrvID.find(nSrvID);
	if( itr != m_SrvBySrvID.end() )
		return itr->second;

	return NULL;
}
