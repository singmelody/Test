#pragma once

#include "BaseType.h"
#include "Time.h"

enum AccountState
{
	eAccountState_Init,
	eAccountState_WaitingNameAndPassword,
	eAccountState_BillingAuth,

	eAccountState_WaitingBilling,
	eAccountState_WaitingWorldSrvCheck,

	eAccountState_WaitingReqEnterGame,

	eAccountState_Destroy,
	eAccountState_Count,
};

class AccountStateBase;

class LoginAccount
{
public:
	LoginAccount(void);
	~LoginAccount(void);

	void OnTick(int32 nFrameTime, Time& tCurr);

	const Time& GetExpireTime() const { return m_expireTime; }

	void HandlePacket(class PacketConnectLoginServer& pkt);
protected:
	AccountStateBase*	m_pState;
	Time				m_expireTime;
	bool				m_bDestroy;
};

