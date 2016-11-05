#pragma once

#include "GateFuncManager.h"
#include "Singleton.h"

class GateSystemManager : public GateFuncManager, public Singleton<GateSystemManager>
{
public:
	GateSystemManager(void);
	~GateSystemManager(void);
};

