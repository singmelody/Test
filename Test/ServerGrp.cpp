#include "StdAfx.h"
#include "ServerGrp.h"
#include "ServerGrpInfo.h"
#include "ServerManager.h"
#include "MyLog.h"
#include "PeerModuleBase.h"

ServerGrp::ServerGrp(void)
{
}


ServerGrp::~ServerGrp(void)
{
}

void ServerGrp::Init(ServerManager* pMgr, int32 nSrvType)
{
	m_pMgr = pMgr;
	m_nSrvType = nSrvType;

	m_strSrvName = GetSrvTitle( nSrvType);
}

ServerInfo* ServerGrp::CreateSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	return new ServerInfo( nSrvType, nSrvID, nSocketID, laddr);
}

ServerInfo* ServerGrp::AddServer(int32 nID, int32 nSocketID, SockAddr& laddr)
{
	if( nID == -1 )
		return NULL;

	ServerInfo* pInfo = GetServer(nID);
	if( pInfo != NULL )
		return NULL;

	MyLog::message("->Server Manager Registry %s id=%d SocketID=%d\n", m_strSrvName.c_str(), nID, nSocketID);

	pInfo = CreateSrvInfo( m_nSrvType, nID, nSocketID, laddr);
	if(!pInfo)
		return NULL;

	pInfo->m_pParentGrp = this;
	m_SrvMap[nID] = pInfo;
	OnAddServer( pInfo );
	m_pMgr->AddSrvInfo(pInfo);
	return pInfo;
}

ServerInfo* ServerGrp::GetServer(int32 nID)
{
	SrvInfoMap::iterator itr = m_SrvMap.find(nID);
	if( itr != m_SrvMap.end() )
		return itr->second;

	return NULL;
}

void ServerGrp::BroadcastPacket(PacketBase* pPkt, int32 nExceptID)
{
	for (auto itr = m_SrvMap.begin(); itr != m_SrvMap.end(); ++itr)
	{
		ServerInfo* pInfo = itr->second;
		if(!pInfo)
			continue;

		if( pInfo->nSrvID == nExceptID )
			continue;

		PeerSend( pInfo->nSocketID, pPkt);
	}
}

void ServerGrp::PeerSend(int32 nSocketID, class PacketBase* pPkt)
{
	if(!m_pMgr || !m_pMgr->m_pPeerModule)
		return;

	m_pMgr->m_pPeerModule->PeerSend( pPkt, nSocketID);
}
