#pragma once

#include "ModuleFuncManager.h"

class WorldFuncManager : public ModuleFuncManager
{
public:
	WorldFuncManager(void);
	virtual ~WorldFuncManager(void);

	void SubWorld2WarWorld(class PacketBase* pPkt);
protected:
	class WorldServer& WorldSrv;
	class WorldAsynContextManager& ContexMgr;
};

