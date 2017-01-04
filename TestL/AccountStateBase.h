#pragma once

#include "LoginFuncManager.h"
#include "PacketBase.h"

class AccountStateBase : public LoginFuncManager
{
public:
	AccountStateBase(void);
	~AccountStateBase(void);

	virtual bool HandlePacket(class PacketConnectLoginServer& pkt){ return false;}
protected:
	void SrvSend(int32 nSocketID, PacketBase& pkt);
	void SrvSend(int32 nSocketID, PacketBase* pPkt);

	class LoginServer& LoginSrv;
};

