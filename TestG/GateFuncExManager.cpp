#include "StdAfx.h"
#include "GateFuncExManager.h"


GateFuncExManager::GateFuncExManager(void)
	: SystemMgr(GateSystemManager::Instance())
	, AccountMgr(GateAccountManager::Instance())
{
}


GateFuncExManager::~GateFuncExManager(void)
{
}
