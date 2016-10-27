#include "StdAfx.h"
#include "PeerBase.h"
#include <assert.h>
#include "WatchDog.h"
#include "NetManager.h"
#include "BaseType.h"

PeerBase::PeerBase(void)
{

}


PeerBase::~PeerBase(void)
{
}

bool PeerBase::PeerInit()
{

}

void PeerBase::FillPeerConfig()
{

}

bool PeerBase::PeerRun()
{
	if(!m_pPeerNetManager)
		return false;

	WatchDog::Instance().NextStep(PEER_THREAD_WATCHDOG_ID);
	m_pPeerNetManager->NetRun();
	WatchDog::Instance().NextStep(PEER_THREAD_WATCHDOG_ID);
}

bool PeerBase::PeerExit()
{

}

void PeerBase::PeerSend(PacketBase* pPkt, int32 nSocketID)
{
	assert( pPkt && m_pPeerNetManager);
	m_pPeerNetManager->SendPacket( *pPkt, nSocketID);
}

void PeerBase::W2LPacketCounter(int32 nPacketType)
{
	if(!W2L_Packet_Counter)
		return;

	PeerSendPacketCounter::iterator itr = m_W2LPacketCounter.find(nPacketType);
	if ( itr == m_W2LPacketCounter.end() )
		m_W2LPacketCounter.insert(std::make_pair< int32, int32>( nPacketType, 1));
	else
		itr->second++;
}
