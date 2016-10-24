#pragma once

#include "WorldFuncExManager.h"
#include "Singleton.h"

class WorldAvatar;
class WorldScene;

class WorldEnterManager : public WorldFuncExManager, public Singleton<WorldEnterManager>
{
public:
	WorldEnterManager(void);
	virtual ~WorldEnterManager(void);

	bool HandleEnterScene( WorldAvatar* pAvatar, WorldScene* pScene);
};

