#include "StdAfx.h"
#include "MyClt.h"


MyClt::MyClt(void)
{
	m_pManager = NULL;

}


MyClt::~MyClt(void)
{
}

void MyClt::Logout()
{
	if(m_nSocketID < 0)
		return;

	m_pManager->m_pNetManager->Disconnect(m_nSocketID);
	m_nSocketID = -1;

	m_bIsLogin = false;
}

void MyClt::SendPacket(PacketBase* pPkt)
{
	if( m_nSocketID >= 0 && m_pManager )
		m_pManager->SendPacket( m_nSocketID, pPkt);
}
