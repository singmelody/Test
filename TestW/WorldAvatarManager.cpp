#include "StdAfx.h"
#include "WorldAvatarManager.h"


WorldAvatarManager::WorldAvatarManager(void)
{
	m_ConnectCount = 0;
}


WorldAvatarManager::~WorldAvatarManager(void)
{
}

WorldAvatar* WorldAvatarManager::CreateWorldAvatar(CreateWorldAvatarArg& arg)
{
	if( m_accountMap.find(arg.strUserAccount) != m_accountMap.end() )
	{
		MyLog::error("WorldAvatarManager::CreateWorldAvatar() Failed for UserCN=[%s] already exist", arg.strUserAccount.c_str());
	}

	WorldAvatar* pAvatar = FACTORY_NEWOBJ(WorldAvatar);
	if(!pAvatar)
	{
		MyLog::error("Create Avatar Failed");
		return NULL;
	}

	if(!pAvatar->Init(arg))
	{
		MyLog::error("Init Avatar Failed");
		return NULL;
	}

	if(!AddPlayerAvatar(pAvatar))
	{
		MyLog::error("Add Avatar 2 Mgr Failed!");
		return NULL;
	}

	m_accountMap.insert(std::make_pair(arg.strUserAccount, pAvatar->GetAvatarNameID()));
	pAvatar->m_account.SetAccountName(arg.strUserAccount);
	pAvatar->SetWorld(&WorldSrv);

	return pAvatar;
}
