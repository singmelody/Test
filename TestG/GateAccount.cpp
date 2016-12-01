#include "StdAfx.h"
#include "GateAccount.h"
#include "GateServer.h"
#include "GateAccountStates.h"
#include "Atomic.h"
#include "MyLog.h"

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

GateAccount* GateAccount::New(int16 nCltSocketID)
{
	return (GateAccount*)FactoryManager_Arg1<int16>::Instance().New("GateAccount", nCltSocketID);
}

void GateAccount::Delete(GateAccount* ptr)
{
	FactoryManager_Arg1<int32>::Instance().Delete(ptr);
}

void GateAccount::ChangeState(GateAccountState nNewState)
{
	GateAccountStateBase* pNewState = GateAccountStateBase::GetState(nNewState);
	if( pNewState == m_pState )
		return;

	m_pState->OnLeave(*this);
	m_pState = pNewState;
	m_pState->OnEnter(*this);
}

void GateAccount::SetExpireTime(uint32 nMilliSeconds)
{
	m_expireTime = Time::CurrentTime() + Time(nMilliSeconds);
}

void GateAccount::Send2Clt(class PacketBase& pkt)
{
	GateServer::Instance().SrvSend( GetCltSocketID(), &pkt);
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

GateAccount* GateAccountManager::CreateAccount(int32 nCltSocketID)
{
	if(m_mapSocketID2Account.find(nCltSocketID) != m_mapSocketID2Account.end() )
	{
		MyLog::message("GateAccountManager::CreateAccount socketID=[%d] already exist", nCltSocketID);
		return NULL;
	}

	GateAccount* pAccount = GateAccount::New(nCltSocketID);
	if(!pAccount)
	{
		MyLog::message("GateAccountManager::CreateAccount socketID=[%d] New GateAccount Failed", nCltSocketID);
		return NULL;
	}

	m_mapSocketID2Account.insert(std::make_pair( nCltSocketID, pAccount));
	return pAccount;
}

void GateAccountManager::RemoveAccount(int32 nCltSocketID)
{
	GateAccountMap::iterator itr = m_mapSocketID2Account.find(nCltSocketID);
	if( itr == m_mapSocketID2Account.end() )
		return;

	int32 nAvatarID = itr->second->GetAvatar().m_nAvatarID;
	m_mapAvatarID2Account.erase( nAvatarID );

	GateAccount::Delete( itr->second );
	m_mapSocketID2Account.erase(nCltSocketID);

	WriteLock tmpLock(&m_RWLock_mapAvatarID2ChannelID);
	m_mapAvatarID2ChannelID.erase(nAvatarID);
}

GateAccount* GateAccountManager::GetAccount(int32 nSocketID)
{
	auto itr = m_mapSocketID2Account.find(nSocketID);
	if( itr == m_mapSocketID2Account.end() )
		return NULL;

	GateAccount* pAccount = itr->second;
	return pAccount;
}

GateAccount* GateAccountManager::GetAccountByAvatarID(int32 nAvatarID)
{
	GateAccountMap::const_iterator itr = m_mapAvatarID2Account.find(nAvatarID);
	if( itr == m_mapAvatarID2Account.end() )
		return NULL;

	GateAccount* pAccount = itr->second;
	return pAccount;
}

void GateAccountManager::Send2AllAvatar(PacketBase& pkt)
{
	for (GateAccountMap::iterator itr = m_mapAvatarID2Account.begin(); itr != m_mapAvatarID2Account.end();++itr)
	{
		GateAccount* pAccount = itr->second;
		if(!pAccount)
			continue;

		pAccount->Send2Clt(pkt);
	}
}
