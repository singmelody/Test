#include "StdAfx.h"
#include "PeerModuleBase.h"
#include "PacketImpl.h"
#include "ConfigManager.h"
#include "MyLog.h"
#include "ServerManager.h"
#include "Thread.h"

PeerModuleBase::PeerModuleBase(SrvType nModuleType) : ModuleBase(nModuleType), m_bUseConnectionThread(false)
{
	Servers.SetPeerModule(this);
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

void PeerModuleBase::Send2Gate(PacketBase* pPkt, int32 nSrvID /*= -1*/, bool bGateProc /*= false*/)
{
	if( !pPkt )
		return;

	if(bGateProc)
		pPkt->SetPacketType(ePacketType_GateProc);

	if( nSrvID != -1 )
	{
		ServerInfo* pInfo = Servers.GetGateInfo(nSrvID);
		if(pInfo)
			PeerSend( pPkt, pInfo->nSocketID);
	}
	else
	{
		if( m_nSrvType == eSrv_Gate )
			nSrvID = m_nSrvID;

		Servers.m_GateGrp.BroadcastPacket( pPkt, nSrvID);
	}

	switch(m_nSrvType)
	{
	case eSrv_World:
		W2GPacketCounter( pPkt->GetPacketID() );
		break;
	case eSrv_Node:
		N2GPacketCounter( pPkt->GetPacketID() );
		break;
	}

}

void PeerModuleBase::Send2Gate(PacketBase* pPkt, int32 nSrvID, uint16 nChannelID, int16 channelList[])
{

}

void PeerModuleBase::StartConnectionThread()
{
	MyThread* pThread = new MyThread("Module Connection");
	pThread->Init(new Function_Arg0<PeerModuleBase>( this, &PeerModuleBase::ConnectThreadProc));
	m_PeerThreadList.add(pThread);
	pThread->Start();
}

void PeerModuleBase::ConnectThreadProc()
{
	ConnectionItem item;

	if(GetConnectionItem(item))
	{
		int32 nSocketID = PeerConnect( item.listenIP, item.nListenPort, false);
		if(nSocketID != -1)
			AddConnectionItem( item.nSrvID, nSocketID);
	}

	Sleep(50);
}

bool PeerModuleBase::GetConnectionItem(ConnectionItem& item)
{

}

void PeerModuleBase::W2GPacketCounter(int32 nPacketType)
{
	if(!W2G_Packet_Counter)
		return;


	PeerSendPacketCounter::iterator itr = m_G2NPacketCounter.find(nPacketType);
	if( itr == m_G2NPacketCounter.end() )
	{
		m_G2NPacketCounter.insert(std::make_pair<int32, int32>( nPacketType, 1));
		return;
	}
	else
		itr->second++;
}

void PeerModuleBase::N2GPacketCounter(int32 nPacketType)
{
	if(!N2G_Packet_Counter)
		return;

	PeerSendPacketCounter::iterator itr = m_N2GPacketCounter.find(nPacketType);
	if( itr == m_N2GPacketCounter.end() )
	{
		m_N2GPacketCounter.insert(std::make_pair<int32, int32>( nPacketType, 1));
		return;
	}
	else
		itr->second++;
}	

void PeerModuleBase::AddConnectionItem(int32 nSrvID, int32 nSocketID)
{
	AUTOLOCK(m_lockConnection);

	ConnectionItem* pItem = new ConnectionItem();

	pItem->nSrvID = nSrvID;
	pItem->nSocketID = nSocketID;

	m_listConnectedItems.push_back(pItem);
}

ConnectionItem* PeerModuleBase::GetConnectedItem()
{
	AUTOLOCK(m_lockConnection);

	ConnectionItem* pItem = NULL;

	ConnectionItemList& list = m_listConnectedItems;
	ConnectionItemList::iterator itr = m_listConnectedItems.begin();
	if( itr != list.end() )
	{
		pItem = *itr;
		list.erase(itr);
	}

	return pItem;
}


void PeerModuleBase::OnPeerDisConnect(int32 nSocketID)
{

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
	memcpy( pItem->listenIpPeer, pPkt->ListenIpPeer, IPLEN);

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

	MyLog::message("Connect 2 %s ip=%s port=%d\n", GetSrvTitle(pItem->nSrvType), ip, nPort);

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

	pkt.nID = m_nSrvID;
	pkt.nType = m_nSrvType;
	pkt.nModuleIndex = -1;

	ConfigManager::GetConfigValue( "CommonConfig", "ModuleIndex", pkt.nModuleIndex, true);
	pkt.nFlag = 0;
	pkt.nListenPortPeer = m_PeerPort;

	memset( pkt.ListenIpPeer, 0, IPLEN);
	memcpy( pkt.ListenIpPeer, m_PeerIP.c_str(), m_PeerIP.length());

	pkt.nListenPortClt = 0;
	memset( pkt.ListenIpClt, 0, IPLEN);

	OnSendPacketSrvConnect(pkt);
	PeerSend( &pkt, nSocketID);
}

void PeerModuleBase::OnSendPacketSrvConnect(PacketSrvConnect& pkt)
{

}

void PeerModuleBase::OnRecvSrvConnectPkt(class PacketSrvConnect* pPkt)
{
	SrvItem item(pPkt);

	MyLog::message("Recv %s Connection id=%d ip=%s port=%d\n", 
		GetSrvTitle(pPkt->type), pPkt->id, pPkt->IP, pPkt->nPort);

	Servers.AddSrvInfo(&item);
}

SrvItem::SrvItem()
{
	nSrvID = 0;
	nSrvType = eSrv_Unkown;

	nSocketID = -1;

	nLastConnectTime = 0;
}

SrvItem::SrvItem(class PacketSrvConnect* pPkt)
{
	nSrvID = pPkt->nID;
	nSrvType = pPkt->nType;

	listenPortPeer = pPkt->nListenPortPeer;
	memcpy( listenIpPeer, pPkt->ListenIpPeer, IPLEN);

	listenPortClt = pPkt->nListenPortClt;
	memcpy( listenIpClt, pPkt->ListenIpClt, IPLEN);

	nSocketID = pPkt->GetSocketID();

	nLastConnectTime = 0;
}
