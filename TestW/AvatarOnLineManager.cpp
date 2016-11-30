#include "StdAfx.h"
#include "AvatarOnLineManager.h"
#include "PacketImpl.h"
#include "WorldAvatar.h"
#include "ConfigManager.h"
#include "MyLog.h"

AvatarOnLineManager::AvatarOnLineManager(void)
	: m_nMaxOnlineUser(0)
	, m_nTimeAccMaxOnlineUser(0)
	, config_MaxOnlineUser(0)
	, config_AccMaxOnlineUser_Enable(false)
	, config_AccMaxOnlineUser_InitValue(0)
	, config_AccMaxOnlineUser_DeltaTime(0)
	, config_AccMaxOnlineUser_Step(0)
{

}


AvatarOnLineManager::~AvatarOnLineManager(void)
{
}

void AvatarOnLineManager::OnConfigLoaded()
{
	ConfigManager::GetConfigValue("WorldConfig", "MaxOnlineUser", config_MaxOnlineUser);

	MyLog::message("MaxOnlineUser=%d", config_MaxOnlineUser);

	SetMaxOnlineUser(config_MaxOnlineUser);
}

void AvatarOnLineManager::Tick(int32 nFrameTime)
{
	ProcOnlineUserInfo(nFrameTime);
}

void AvatarOnLineManager::ProcOnlineUserInfo(int32 nFrameTime)
{
	static int32 s_interval = 0;
	s_interval += nFrameTime;

	// 10s sync once
	if(s_interval < 10*1000)
		return;

	SyncOnlineUserInfo2Login();
	s_interval = 0;
}

void AvatarOnLineManager::SyncOnlineUserInfo2Login()
{
	PacketOnlineUserInfo pkt;
	pkt.nMaxCount = GetMaxOnlineUser();
	pkt.nCount = GetOnlineAvatarCount();
	Send2Login(pkt);
}

void AvatarOnLineManager::AddAccount(WorldAvatar* pAvatar)
{
	if( !pAvatar )
		return;

	const std::string& strAccountName = pAvatar->Account.GetAccountName();

	assert(!strAccountName.empty());

	if(strAccountName.empty())
		return;

	m_accountMap.insert( std::make_pair( strAccountName, pAvatar->GetAvatarID()));
	return;
}

void AvatarOnLineManager::DelAccount(WorldAvatar* pAvatar)
{
	if( !pAvatar )
		return;

	const std::string& strAccountName = pAvatar->Account.GetAccountName();
	assert(!strAccountName.empty());
	if( (pAvatar->GetName() && (m_avatarMap.find(pAvatar->GetName()) != m_avatarMap.end())) 
		|| ( m_avatarDIDTable.find( pAvatar->GetAvatarDID()) != m_avatarDIDTable.end()) 
		|| (pAvatar->GetName() && (m_avatarNameTable.find(pAvatar->GetName()) != m_avatarNameTable.end())))
	{
		return;
	}

	auto itr = m_accountMap.find(strAccountName);
	if( itr == m_accountMap.end())
		return;

	m_accountMap.erase(itr);

}

void AvatarOnLineManager::AddAvatar(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return;

	if(m_accountMap.find(pAvatar->GetAccountName()) != m_accountMap.end())
		return;

	const int32 nID = pAvatar->GetAvatarID();

	std::string name = pAvatar->GetName();

	m_avatarMap.insert( std::make_pair( name, pAvatar->GetAvatarID() ));
	m_avatarDIDTable.insert( std::make_pair(pAvatar->GetAvatarDID(), pAvatar->GetAvatarID()) );
	m_avatarNameTable.insert( std::make_pair( name, pAvatar->GetAvatarID()));
}

void AvatarOnLineManager::DelAvatar(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return;

	if(!pAvatar->GetAccountName() || !pAvatar->GetName())
		return;

	auto accountItr = m_accountMap.find(pAvatar->GetAccountName());
	if(accountItr != m_accountMap.end())
		m_accountMap.erase(accountItr);

	auto avatarItr = m_avatarMap.find(pAvatar->GetName());
	if(avatarItr != m_avatarMap.end())
		m_avatarMap.erase(avatarItr);

	auto didItr = m_avatarDIDTable.find(pAvatar->GetAvatarDID());
	if( didItr != m_avatarDIDTable.end() )
		m_avatarDIDTable.erase(didItr);


	auto nameItr = m_avatarNameTable.find(pAvatar->GetName());
	if( nameItr != m_avatarNameTable.end() )
		m_avatarNameTable.erase(nameItr);
}

int32 AvatarOnLineManager::GetOnlineAvatarCount()
{
	return (int32)m_avatarMap.size();
}

WorldAvatar* AvatarOnLineManager::GetAvatarByAccountName(const char* accountName)
{
	if(accountName == NULL)
		return NULL;

	auto itr = m_accountMap.find(accountName);
	if(itr == m_accountMap.end())
		return NULL;

	int32 tAvatarID = itr->second;
	WorldAvatar* pAvatar = GetWorldAvatar(tAvatarID);
	if(!pAvatar)
	{
		m_accountMap.erase(itr);
		return NULL;
	}

	return pAvatar;
}

WorldAvatar* AvatarOnLineManager::GetAvatarByDID(int64 nAvatarDID)
{
	auto itr = m_avatarDIDTable.find(nAvatarDID);
	if( itr == m_avatarDIDTable.end() )
		return NULL;

	int32 nAvatarID = itr->second;
	WorldAvatar* pAvatar = GetWorldAvatar(nAvatarID);
	return pAvatar;
}

WorldAvatar* AvatarOnLineManager::GetAvatarByName(const char* name)
{
	AvatarNameMap::iterator itr = m_avatarNameTable.find(name);
	if( itr != m_avatarNameTable.end())
		return GetWorldAvatar(itr->second);

	return NULL;
}