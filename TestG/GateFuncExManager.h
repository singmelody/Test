#pragma once

class GateFuncExManager : public GateFuncManager
{
public:
	GateFuncExManager(void);
	~GateFuncExManager(void);

protected:
	GateAccountManager& AccountMgr;
	GateSystemManager& SystemMgr;
};

