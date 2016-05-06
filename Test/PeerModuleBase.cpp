#include "StdAfx.h"
#include "PeerModuleBase.h"
#include "MyLog.h"

PeerModuleBase::PeerModuleBase(int32 nModuleType)
	: ModuleBase(nModuleType)
	, m_bUseConnectionThread(false)
	, Servers.SetPeerModule(this)
{

}


PeerModuleBase::~PeerModuleBase(void)
{
}

bool PeerModuleBase::Init(int32 argc, char* argv[])
{
	if( !ModuleBase::Init( argc, argv))
		return false;

	if(!PeerInit())
		return false;

	if( m_bUseConnectionThread)
		StartConnectionThread();

	return true;
}

void PeerModuleBase::OnAddServerInfo(ServerInfo* pInfo)
{
	if (!pInfo)
		return;

	switch(pInfo->nSrvType)
	{
	case eSrv_World:
		OnAddWorldInfo(pInfo);
		break;

	case eSrv_Login:
		OnAddLoginInfo(pInfo);
		break;
	case eSrv_DBA:
		OnAddDBAInfo(pInfo);
		break;
	case eSrv_Gate:
		OnAddGateInfo(pInfo);
		break;

	case eSrv_Node:
		OnAddNodeInfo(pInfo);
		break;
	}
}

void PeerModuleBase::OnRemoveServerInfo(ServerInfo* pInfo)
{
	if (!pInfo)
		return;

	switch(pInfo->nSrvType)
	{
	case eSrv_World:
		OnRemoveWorldInfo(pInfo);
		break;

	case eSrv_Login:
		OnRemoveLoginInfo(pInfo);
		break;
	case eSrv_DBA:
		OnRemoveDBAInfo(pInfo);
		break;
	case eSrv_Gate:
		OnRemoveGateInfo(pInfo);
		break;

	case eSrv_Node:
		OnRemoveNodeInfo(pInfo);
		break;
	}
}

void PeerModuleBase::Send2Login(PacketBase* pPkt)
{
	if(!pPkt)
		return;

	ServerInfo* pInfo = Servers.GetLoginInfo();
	if(pInfo)
		return;

	PeerSend( pPkt, pInfo->nSocketID);

	switch(m_nSrvType)
	{
	case eSrv_World:
		W2LPacketCounter(pPkt->GetPacketID());
		break;
	}
}

void PeerModuleBase::StartConnectionThread()
{
	MyThread* pThread = new MyThread("Module Connection");
	pThread->Init(new FunctionBase_Arg0<PeerModuleBase>( this, &PeerModuleBase::ConnectThreadProc));
	m_PeerThreadList.push_back(pThread);
	pThread->Start();
}

void PeerModuleBase::ConnectThreadProc()
{
	ConnectionItem item;

	if(GetConnectionItem(item))
	{
		int32 nSocketID = PeerConnect( item.listenIP, item.listenPort, false);
		if(nSocketID != -1)
			AddConnectionItem( item.nSrvID, nSocketID);
	}

	Sleep(50);
}

bool PeerModuleBase::GetConnectionItem(ConnectionItem& item)
{

}

void PeerModuleBase::OnPeerDisConnect(int32 nSocketID)
{
	_FunctionStart( PeerModuleBase_OnPeerDisConnect);

	ServerInfo* pInfo = Servers.GetSrvBySocketID( nSocketID );
	if(!pInfo)
	{
		MyLog::error("Lose Connection socket_id = %d\n", nSocketID);
		return;
	}

	MyLog::error( "Lose %s Connection socket id [%d]", GetSrvTitle(pInfo->nSrvType), nSocketID);

	switch(pInfo->nSrvType)
	{
	case eSrv_World:
		OnWorldDisconnect(pInfo);

		if( Servers.m_pLocalWorld == pInfo )
		{
			OnLocalWorldDisconnect(pInfo);
			Servers.m_pLocalWorld = NULL;
			Servers.m_localWorldChannelID = -1;
		}
		break;
	case eSrv_DBA:
		if(Servers.m_pDBA == pInfo)
		{
			OnDBADisconnect(pInfo);
			Servers.m_pDBA = NULL;
		}
		break;

	case eSrv_Login:
		if(Servers.m_pLogin == pInfo)
		{
			OnLoginDisconnect(pInfo);
			Servers.m_pLogin = NULL;
		}
		break;

	case eSrv_Gate:
		OnGateDisconnect(pInfo);
		break;

	case eSrv_Node:
		OnNodeDisconnect(pInfo);
		break;

	case eSrv_GMI:
		if(Servers.m_pGMI == pInfo)
			Servers.m_pGMI = NULL;
		break;
	}

	if( pInfo->m_pParentGrp != NULL)
		pInfo->m_pParentGrp->RemoveServer(pInfo->nSrvID);

	Servers.RemoveSrvInfo(pInfo);

	SAFE_DELETE(pInfo);

	_FunctionEnd();
}

void PeerModuleBase::OnRecvSrvInfoPkt(PacketAddSrvInfo* pPkt)
{
	if( !pPkt)
		return;

	MyLog::message("Recv Srv Info type[%s] ip[%s]", GetSrvTitle(pPkt->nSrvType), pPkt->nListenPortPeer);

	SrvItem* pItem = new SrvItem();

	pItem->nSrvType = pPkt->nSrvType;
	pItem->nSrvID = pPkt->nSrvID;

	pItem->listenPortPeer = pPkt->nListenPortPeer;
	memcpy( pItem->listenIpPeer, pPkt->listenIpPeer, IPLEN);

	pItem->listenPortClt = pPkt->nListenPortClt;
	memcpy( pItem->listenIpClt, pPkt->ListenIpClt, IPLEN);

	m_mapSrvItems[pItem->nSrvID] = pItem;

	if(!m_bUseConnectionThread)
		SyncConnectServer(pItem);
}

void PeerModuleBase::SyncConnectServer(SrvItem* pItem)
{
	char* ip = pItem->listenIpPeer;
	int32 nPort = pItem->listenPortPeer;

	MyLog::message("Connect 2 %s ip=%s port=%d\n", GetServerTitle(pItem->nSrvType), ip, nPort);

	int32 nSocketID = PeerConnect( ip, nPort, true);
	if(nSocketID == -1)
		return;

	pItem->nSocketID = nSocketID;
	OnPeerConnected(pItem);
}

void PeerModuleBase::OnPeerConnected(SrvItem* pItem)
{
	SendPacketSrvConnect( pItem->nSocketID );
	Servers.AddSrvInfo(pItem);
}

void PeerModuleBase::SendPacketSrvConnect(int32 nSocketID)
{
	MyLog::message( "PeerModuleBase::SendPacketSrvConnect=%d\n", nSocketID);

	PacketSrvConnect pkt;

	pkt.id = m_nSrvID;
	pkt.type = m_nSrvType;
	pkt.moduleindex = -1;

	ConfigManager::GetConfigValue( "CommonConfig", "ModuleIndex", pkt.moduleindex, true);
	pkt.flag = 0;
	pkt.listenPortPeer = m_peerPort;

	memset( pkt.listenIpPeer, 0, IPLEN);
	memcpy( pkt.litenIpPeer, m_peerIP.c_str(), m_peerIP.length());

	pkt.listenPortClt = 0;
	memset( pkt.listenIpClt, 0, IPLEN);

	OnSendPacketSrvConnect(pkt);
	PeerSend( &pkt, nSocketID);
}

void PeerModuleBase::OnSendPacketSrvConnect(PacketSrvConnect& pkt)
{

}

void PeerModuleBase::OnRecvSrvConnectPkt(class PacketSrvConnect* pPkt)
{
	SrvItem item( pPkt);

	MyLog::message("Recv %s Connection id=%d ip=%s port=%d\n", 
		GetSrvTitle(pPkt->type), pPkt->id, pPkt->IP, pPkt->nPort);

	Servers.AddSrvInfo(&item);
}

