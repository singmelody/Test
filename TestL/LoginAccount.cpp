#include "StdAfx.h"
#include "LoginAccount.h"


LoginAccount::LoginAccount(void)
{
}


LoginAccount::~LoginAccount(void)
{
}

void LoginAccount::HandlePacket(class PacketConnectLoginServer& pkt)
{
	if(!m_pState)
		return;

	if(!m_pState->HandlePacket( *this, pkt))
		ChangeState(eAccountState_Destroy);
}
