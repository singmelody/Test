#include "LoginStateManager.h"
#include "LoginStateManager.h"
#include "StdAfx.h"


LoginStateManager::LoginStateManager(void)
{
	s_states[eAccountState_Init] = AccountState_Init::InstancePtr();
	s_states[eAccountState_WaitingNameAndPassword] = AccountState_Init
}


LoginStateManager::~LoginStateManager(void)
{
}
