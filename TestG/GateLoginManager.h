#pragma once
#include "GateFuncExManager.h"
#include "Singleton.h"

class GateLoginManager : public GateFuncExManager, public Singleton<GateLoginManager>
{
public:
	GateLoginManager(void);
	~GateLoginManager(void);
};

