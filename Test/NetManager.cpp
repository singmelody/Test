#include "StdAfx.h"
#include "NetManager.h"
#include "NetChannel.h"
#include <assert.h>

NetManager::NetManager(void)
{
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

}

int32 NetManager::Connect( char* sAddr, int32 nPort, bool bSingleThread)
{
	NetChannelBase* pBase = CreateNewChannel();
	if(!pBase)
		return -1;



	return -1;
}

NetChannelBase* NetManager::CreateNewChannel()
{
	return new NetChannel();
}

void NetManager::ProcNewConnection()
{

}

void NetManager::ProcPendingClose()
{

}

void NetManager::ProcPendingDestroy()
{

}

void NetManagerReactor::NetRun()
{
	NetManager::NetRun();

	if(!m_pReactor)
		return;

	m_pReactor->HandleEvents(m_Timer);

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
	m_pReactor->RegisterHandler( handler, NETEVENT_IO);	
}


void NetManagerReactor::OnDisconnect(NetChannelBase* pChannel)
{
	if(!pChannel || !m_pReactor)
		return;

	NetEventHandler* pHandler = static_cast<NetEventHandler*>(pChannel);
	m_pReactor->RemoveHandler(pHandler);

	NetManager::OnDisconnect(pChannel);
}

NetManagerIOCP::NetManagerIOCP(bool bLZOCompress, int32 nSockRcBuffSize, int32 nRcBufferSize, int32 nSnBufferSize, FunctionBase_Arg1<int32>* funcAccept /*= NULL*/, FunctionBase_Arg1<int32>* funcCon /*= NULL*/, FunctionBase_Arg1<int32>* funcDiscon /*= NULL*/, int32 MAX_SOCKETS /*= MY_SOCKET_LIST_SIZE */)
{

}
