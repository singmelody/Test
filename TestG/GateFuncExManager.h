#pragma once

#include "GateFuncManager.h"

class GateFuncExManager : public GateFuncManager
{
public:
	GateFuncExManager(void);
	~GateFuncExManager(void);

protected:
	class GateAccountManager& AccountMgr;
	class GateSystemManager& SystemMgr;
};

