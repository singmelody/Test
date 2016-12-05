#include "StdAfx.h"

#include "NetManager.h"
#include "NetChannel.h"
#include "NetReactor.h"
#include <assert.h>
#include "MyLog.h"
#include "PacketProcessor.h"
#include "PacketFactory.h"

NetManager::NetManager(
	bool bLZOCompress, 
	int32 nSockRcBufSize, 
	int32 nRcBufSize, 
	int32 nSockSnBufSize, 
	int32 nSnBufSize, 
	FunctionBase_Arg1<int32>* funcAccept, 
	FunctionBase_Arg1<int32>* funcCon, 
	FunctionBase_Arg1<int32>* funcDiscon,
	int32 MAX_SOCKETS)
	:m_pProcessor(NULL)
	,m_ChannelList(MAX_SOCKETS)
	,m_bLzoCompress(bLZOCompress)
	,m_pIRecvPacketFilter(0)
	,m_bUseIndexWhenSend(false)
	,m_bUseIndexWhenRecv(false)
{
	m_nSockRcBufSize = nSockRcBufSize;
	m_nRcBufSize = nRcBufSize;
	m_nSockRnBufSize = nSockSnBufSize;
	m_nSnBufSize = nSnBufSize;

}

NetManager::~NetManager(void)
{
}

void NetManager::NetRun()
{
	ProcNewConnection();
	ProcPendingClose();
	ProcPendingDestroy();
}

void NetManager::Accept( char* sAddr, int32 nPort, bool bSrv)
{
	m_Accept.Init( this, m_nSockRcBufSize, m_nRcBufSize, m_nSockRnBufSize, m_nSnBufSize);
	m_Accept.Accept( sAddr, nPort);
}

int32 NetManager::Connect( char* sAddr, int32 nPort, bool bSingleThread)
{
	NetChannelBase* pBase = CreateNewChannel();
	if(!pBase)
		return -1;

	int32 nChannelID = -1;
	pBase->InitChannel( this, m_nSockRcBufSize, m_nRcBufSize, m_nSockRnBufSize, m_nSnBufSize);

	if(pBase->Connect( sAddr, nPort))
	{
		if(bSingleThread)
		{
			AddNewConnection( pBase, false, false);
			ProcNewConnection();
		}
		else
		{
			AddNewConnection( pBase, false, true);
		}

		nChannelID = pBase->GetID();
	}

	FreeChannel( pBase );

	return nChannelID;
}

void NetManager::CloseChannel(int32 nChannel)
{
	NetChannelBase* pChannel = NULL;

	m_mutexChannels.Lock();

	pChannel = m_ChannelList.XGet( nChannel );

	m_ChannelList.remove( nChannel);

	m_mutexChannels.Unlock();

	if( pChannel == NULL || pChannel->GetSocket() == INVALID_SOCKET )
		return;

	if(m_DisconnectCallBack)
		(*m_DisconnectCallBack)(nChannel);

	OnDisconnect(pChannel);
	pChannel->CloseChannel();
	FreeChannel(pChannel);
}

void NetManager::OnDisconnect(NetChannelBase* s)
{

}

int32 NetManager::AddChannel(NetChannelBase* pBase)
{
	AUTOLOCK(m_mutexChannels);

	int32 nSocketID = -1;
	nSocketID = m_ChannelList.add( pBase );
	if( nSocketID >= 0 )
	{
		pBase->SetID( nSocketID );
		pBase->m_RefCount++;
	}

	return nSocketID;
}

int32 NetManager::AddNewConnection(NetChannelBase* pBase, bool bSocketIsAccepted, bool bWait)
{
	if(!pBase)
		return -1;

	pBase->SetID(-1);
	pBase->m_bCreateByAccept = bSocketIsAccepted;

	if(bWait)
		pBase->m_hProcNewNotify.CreateEvent( false, false);

	{
		AUTOLOCK( m_mutexChannels );
		m_newSocketQueue.push_back(pBase);
		pBase->m_RefCount++;
	}

	int32 nChannelID = -1;
	if(bWait)
	{
		pBase->m_hProcNewNotify.Wait();
		nChannelID = pBase->GetID();
	}

	return nChannelID;
}

void NetManager::FreeChannel(NetChannelBase* pBase)
{
	AUTOLOCK(m_mutexChannels);

	if(pBase != NULL)
	{
		pBase->m_RefCount--;
		if( pBase->m_RefCount <= 0 )
		{
			AUTOLOCK( m_destroyQueueLock );
			m_destroyQueue.push_back(pBase);
		}
	}
}

NetChannelBase* NetManager::GetChannel(int32 nSocketID)
{
	AUTOLOCK(m_mutexChannels);

	if( nSocketID < 0 || nSocketID >= m_ChannelList.GetMaxElement() )
		return NULL;

	NetChannelBase* pChannel = m_ChannelList.XGet(nSocketID);
	if(!pChannel || pChannel->m_bIsClosing)
		return NULL;

	pChannel->m_RefCount++;
	return pChannel;
}

bool NetManager::SendPacket(PacketBase& pkt, int32 nSocketID)
{
	bool bSuceed = false;

	NetChannelBase* pChannel = GetChannel( nSocketID );
	if ( pChannel )
	{
		PacketBase* pPkt = DuplicatePacket(pkt);
		if(!pPkt)
			MyLog::error("NetManager::SendPacket failed to DuplicatePacket [%s]", pkt.GetClassName() );
		else
		{
			pPkt->SetSocketID( nSocketID );
			pChannel->AppendPacket( pPkt );
			bSuceed = true;
		}
		FreeChannel(pChannel);
	}

	return bSuceed;
}

void NetManager::OnReceivedPacket(NetChannelBase* pChannel, PacketBase* pPkt)
{
	if(!m_pProcessor)
		return;

	m_pProcessor->OnPacketReceived( pPkt );
}

NetChannelBase* NetManager::CreateNewChannel()
{
	return new NetChannel();
}

PacketBase* NetManager::DuplicatePacket(PacketBase& pkt)
{
	PacketBase* pPktTmp = (PacketBase*)PacketFactory::Instance().New( pkt.GetPacketID() );
	if( pPktTmp == NULL )
		return 0;

#if PACKET_USE_INDEX_DATA
	bool bOri = pkt.IsUseIndex();
	pkt.IsUseIndex(false);
	pPktTmp->IsUseIndex(false);
#endif

	char buffer[PACKET_MAX_SIZE];
	pkt.WritePacket(buffer);
	pPktTmp->ReadPacket(buffer);

#if PACKET_USE_INDEX_DATA
	pkt.IsUseIndex(bOri);
#endif

	return pPktTmp;
}

void NetManager::Disconnect(int32 nSocketID)
{
	AUTOLOCK(m_closingQueueLock);
	m_closingQueue.push_back(nSocketID);
}

void NetManager::ProcNewConnection()
{
	std::deque<NetChannelBase*> tmpQueue;
	
	{
		AUTOLOCK( m_mutexChannels);
		tmpQueue.swap(m_newSocketQueue);
	}

	while (!tmpQueue.empty())
	{
		NetChannelBase* pChannel = tmpQueue.front();
		assert( pChannel);

		int32 nSocketID = AddChannel( pChannel );
		if(nSocketID < 0)
		{
			// channel list is full , can't add another one
			MyLog::warning("NetManager::ProcNewConnection() socket list is full");
			break;
		}

		pChannel->StartChannel();
		OnChannelStart( pChannel );

		if(pChannel->m_bCreateByAccept)
		{
			if(m_AcceptCallBack)
				(*m_AcceptCallBack)(nSocketID);
		}
		else
		{
			if(m_ConnectCallBack)
				(*m_ConnectCallBack)(nSocketID);
		}

		tmpQueue.pop_front();

		if(pChannel->m_hProcNewNotify.IsValid())
			pChannel->m_hProcNewNotify.SetEvent();

		FreeChannel( pChannel );
	}

	// failed process
	while (!tmpQueue.empty())
	{
		NetChannelBase* pChannel = tmpQueue.front();
		tmpQueue.pop_front();
		assert(pChannel);

		if(pChannel->m_hProcNewNotify.IsValid())
		{
			AUTOLOCK(m_mutexChannels);
			m_newSocketQueue.push_back( pChannel );
		}
		else
		{
			SockAddr addr;
			pChannel->GetAddr( addr );
			MyLog::warning("NetManager::ProcNewConnection() drop new socket (%s:%d)", addr.GetIP().c_str(), addr.GetPort());
			FreeChannel(pChannel);
		}
	}
}

void NetManager::ProcPendingClose()
{
	std::deque<int32> vToClose;

	{
		AUTOLOCK(m_closingQueueLock);

		while (!m_closingQueue.empty())
		{
			int32 nChannelID = m_closingQueue.front();
			m_closingQueue.pop_front();
			vToClose.push_back(nChannelID);
		}
	}

	for (auto itr = vToClose.begin(); itr != vToClose.end(); ++itr)
	{
		CloseChannel(*itr);
	}
}

void NetManager::ProcPendingDestroy()
{
	AUTOLOCK(m_destroyQueueLock);

	while (!m_destroyQueue.empty())
	{
		NetChannelBase* pChannel = m_destroyQueue[0];
		SAFE_DELETE(pChannel);
		m_destroyQueue.pop_front();
	}
}


NetManagerIOCP::NetManagerIOCP(
	bool bLZOCompress, int32 nSockRcBuffSize, int32 nRcBufferSize, int32 nSockSnBufferSize, 
	int32 nSnBufferSize, FunctionBase_Arg1<int32>* funcAccept /*= NULL*/, FunctionBase_Arg1<int32>* funcCon /*= NULL*/, 
	FunctionBase_Arg1<int32>* funcDiscon /*= NULL*/, int32 MAX_SOCKETS /*= MY_SOCKET_LIST_SIZE */)
	: NetManager( bLZOCompress, nSockRcBuffSize, nRcBufferSize, nSockSnBufferSize, 
				nSnBufferSize, funcAccept, funcCon, funcDiscon, MAX_SOCKETS)
{

}


NetManagerReactor::NetManagerReactor(
	bool bLZOCompress, int32 nSockRcBuffSize, int32 nRcBufferSize, int32 nSockSnBufferSize, 
	int32 nSnBufferSize, FunctionBase_Arg1<int32>* funcAccept, FunctionBase_Arg1<int32>* funcCon, 
	FunctionBase_Arg1<int32>* funcDiscon, int32 MAX_SOCKETS)
	: NetManager( bLZOCompress, nSockRcBuffSize, nRcBufferSize, nSockSnBufferSize, 
	nSnBufferSize, funcAccept, funcCon, funcDiscon, MAX_SOCKETS)
{

}

void NetManagerReactor::NetRun()
{
	NetManager::NetRun();

	if(!m_pReactor)
		return;

	m_pReactor->HandleEvents(m_timer, GetEventWaitTime());

	const int32 MAX_SOCKETS = m_ChannelList.GetMaxElement();
	for (int32 i = 0; i < MAX_SOCKETS; ++i)
	{
		NetChannelBase* pChannel = GetChannel(i);
		if(!pChannel)
			continue;

		pChannel->HandleOutput();
		FreeChannel(pChannel);
	}
}

void NetManagerReactor::OnChannelStart(NetChannelBase* pChannel)
{
	assert(m_pReactor);
	if(!m_pReactor)
		return;

	NetEventHandler* pHandler = static_cast<NetEventHandler*>(pChannel);
	m_pReactor->RegisterHandler( pHandler, eNEM_IO);	
}


void NetManagerReactor::OnDisconnect(NetChannelBase* pChannel)
{
	if(!pChannel || !m_pReactor)
		return;

	NetEventHandler* pHandler = static_cast<NetEventHandler*>(pChannel);
	m_pReactor->RemoveHandler(pHandler);

	NetManager::OnDisconnect(pChannel);
}