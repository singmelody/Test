#pragma once

#include "Singleton.h"
#include "SceneManager.h"
#include "WorldFuncManager.h"

class WorldSceneManager : public SceneManager, public WorldFuncManager, Singleton<WorldSceneManager>
{
public:
	WorldSceneManager(void);
	~WorldSceneManager(void);

	WorldAvatarManager& AvatarMgr;

	virtual void OnSceneCreated( Scene* pScene);
	virtual void OnSceneDestroy( Scene* pScene);

	virtual SceneInfo* CreateSceneInfo( int32 );
};

