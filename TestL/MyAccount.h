#pragma once

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

class MyAccount
{
public:
	MyAccount(void);
	~MyAccount(void);
};

