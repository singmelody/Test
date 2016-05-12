#include "StdAfx.h"
#include "PeerBase.h"


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
