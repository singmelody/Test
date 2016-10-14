#include "StdAfx.h"
#include "GateAccount.h"
#include "GateServer.h"
#include "GateAccountStates.h"
#include "Atomic.h"

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

class GateCltNetChannel* GateAccountManager::GetCltChannelByAvatarID(int32 nAvatarID)
{
	int32 nChannelID = GetCltChannelID( nAvatarID );
	if( nChannelID < 0 )
		return NULL;

	return GateServer::Instance().GetCltChannel( nChannelID );
}

int32 GateAccountManager::GetCltChannelID(int32 nAvatarID)
{
	int32 nChannelID = -1;
	ReadLock tmp(&m_RWLock_mapAvatarID2ChannelID);

	stdext::hash_map<int32, int16>::iterator itr = m_mapAvatarID2ChannelID.find(nAvatarID);
	if( itr != m_mapAvatarID2ChannelID.end() )
		nChannelID = itr->second;

	return nChannelID;
}
