#pragma once

#include "LoginFuncManager.h"

class AccountStateBase : public LoginFuncManager
{
public:
	AccountStateBase(void);
	~AccountStateBase(void);

protected:
	void SrvSend(int32 nSocketID, PacketBase& pkt);
	void SrvSend(int32 nSocketID, PacketBase* pPkt);

	class LoginServer& LoginSrv;
};

