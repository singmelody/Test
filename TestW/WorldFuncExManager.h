#pragma once

#include "WorldFuncManager.h"

class WorldFuncExManager : public WorldFuncManager
{
public:
	WorldFuncExManager(void);
	virtual ~WorldFuncExManager(void);

protected:
	void Send2WarWorld(PacketBase* pPkt);

	class WorldAvatarManager& AvatarMgr;
	class WorldSceneManager& SceneMgr;
};

