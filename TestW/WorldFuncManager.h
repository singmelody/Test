#pragma once

#include "ModuleFuncManager.h"

class WorldFuncManager : public ModuleFuncManager
{
public:
	WorldFuncManager(void);
	virtual ~WorldFuncManager(void);

protected:
	class WorldServer& WorldSrv;
	class WorldAyncContextManager& ContexMgr;
};

