#pragma once
#include "AccountStateBase.h"
#include "Singleton.h"
#include "LoginAccount.h"

class AccountState_Destroy : public AccountStateBase, public Singleton<AccountState_Destroy>
{
public:
	AccountState_Destroy(void);
	virtual ~AccountState_Destroy(void);

	virtual void OnEnter(LoginAccount& account)
	{
		account.SetExpireTime(5*1000);
	}

	virtual void OnExpire(LoginAccount& account)
	{
		if(account.IsDestroy())
			return;

		PacketCloseCommand closePkt;
		account.SendPacket(&closePkt);
		DestroyAccount(account);
	}
};

