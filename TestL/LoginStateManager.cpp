#include "LoginStateManager.h"
#include "LoginStateManager.h"
#include "StdAfx.h"


LoginStateManager::LoginStateManager(void)
{
	s_states[eAccountState_Init] = AccountState_Init::InstancePtr();
	s_states[eAccountState_WaitingNameAndPassword] = AccountState_WaitingNameAndPassword::InstancePtr();
	s_states[eAccountState_BillingAuth] = AccountState_BillingAuth::InstancePtr();
	s_states[eAccountState_WaitingBilling] = AccountState_WaitingBilling::InstancePtr();
	s_states[eAccountState_WaitingWorldSrvCheck] = AccountState_WaitingWorldSrvCheck::InstancePtr();
	s_states[eAccountState_WaitingReqEnterGame] = AccountStateWaitingReqEnterGame::InstancePtr();
	s_states[eAccountState_Destroy] = AccountState_Destroy::InstancePtr();

}


LoginStateManager::~LoginStateManager(void)
{
}
