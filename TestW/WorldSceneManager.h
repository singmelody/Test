#pragma once

#include "Singleton.h"
#include "SceneManager.h"
#include "WorldFuncManager.h"
#include "BaseType.h"

class Scene;
class SceneInfo;
class WorldScene;
class WorldAvatarManager;

class WorldSceneManager : public SceneManager, public WorldFuncManager, Singleton<WorldSceneManager>
{
public:
	WorldSceneManager(void);
	~WorldSceneManager(void);

	WorldAvatarManager& AvatarMgr;

	virtual void OnSceneCreated( Scene* pScene);
	virtual void OnSceneDestroy( Scene* pScene);

	virtual SceneInfo* CreateSceneInfo( int32 nSceneType, int32 nPlayerMax);
	void OnCreatingSceneTimeout( WorldScene* pScene);
};

