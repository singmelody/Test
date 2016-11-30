#include "StdAfx.h"
#include "WorldAvatarManager.h"
#include "WorldAvatar.h"
#include "MyLog.h"
#include "AvatarOnLineManager.h"
#include "WorldSceneManager.h"

WorldAvatarManager::WorldAvatarManager(void)
	: SceneMgr(WorldSceneManager::Instance())
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

	m_accountMap.insert(std::make_pair(arg.strUserAccount, pAvatar->GetAvatarID()));
	pAvatar->m_account.SetAccountName(arg.strUserAccount);
	pAvatar->SetWorld(&WorldSrv);

	return pAvatar;
}

void WorldAvatarManager::RemoveWorldAvatar(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return;

	m_accountMap.erase(pAvatar->Account.GetAccountName());
	AvatarOnLineManager::Instance().DelAvatar(pAvatar);

	int32 nAvatarID = pAvatar->GetAvatarID();

	MyLog::message("WorldRemoveAvatar [%]", nAvatarID);

	if (pAvatar->HasGateAvatar())
	{
		PacketGateDestroyAvatar pkt;
		pAvatar->Send2Gate( &pkt, true);
	}
	pAvatar->SetCurState( eWS_Null);
	RemovePlayerAvatar( nAvatarID );
	FACTORY_DELOBJ(pAvatar);
}

int32 WorldAvatarManager::CheckAccountOnline(const char* pAccout, int32& nAvatarID)
{
	if(!pAccout)
		return -1;

	WorldAvatar* pAvatar = AvatarOnLineManager::Instance().GetAvatarByAccountName(pAccout);
	if(pAvatar)
	{
		nAvatarID = pAvatar->GetAvatarID();
		if( pAvatar->m_nCurStageID == eWS_ExitGame )
			return -3;	// should enter waitlogining state, wait data save finish

		return -4;
	}

	nAvatarID = -1;
	return 0;
}
