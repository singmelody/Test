#pragma once

#include "GateFuncManager.h"
#include "GateAccount.h"
#include "GateSystemManager.h"

class GateFuncExManager : public GateFuncManager
{
public:
	GateFuncExManager(void);
	~GateFuncExManager(void);

protected:
	class GateAccountManager& AccountMgr;
	class GateSystemManager& SystemMgr;
};

