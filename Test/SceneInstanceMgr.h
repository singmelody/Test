#pragma once

#include "ID2ItemMap.h"

class Scene;
class SceneInfo;

class SceneInstanceMgr : public ID2ItemMap<int32, Scene>
{
public:
	SceneInstanceMgr(void);
	~SceneInstanceMgr(void);

	uint32 AllocSceneID(bool bRand);
	virtual bool	InitScene(Scene* pScene);

	Scene*	GetMinLoadScene();
	Scene*  GetFrontScene();

	const SceneInfo* m_pSceneInfo;
};

