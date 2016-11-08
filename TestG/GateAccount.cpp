#include "StdAfx.h"
#include "GateAccount.h"

FINISH_FACTORY_ARG1(GateAccount);

GateAccount::GateAccount(int16 nCltSocketID)
	:m_nClntSockID(nCltSocketID)
	,m_bClntConnect(true)
	,m_bDestroy(false)
	,m_bNeedNoticeWorldWhenDestroy(false)
	,m_nUserIP(0)
	,m_nCityChatChannelID()
{
	m_pState = GateAccountStateBase::GetState(eGateAccountState_CltConnected);
	m_pState->OnEnter(*this);
}


GateAccount::~GateAccount(void)
{
}

GateAccountManager::GateAccountManager()
{

}

GateAccountManager::~GateAccountManager()
{

}
