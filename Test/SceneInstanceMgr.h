#pragma once

#include "ID2ItemMap.h"
#include "Scene.h"

class SceneInstanceMgr : public ID2ItemMap<int32, Scene>
{
public:
	SceneInstanceMgr(void);
	~SceneInstanceMgr(void);

	uint32 AllocSceneID(bool bRand);

	const Scene*	m_pSceneInfo;
	virtual bool	InitScene(Scene* pScene);

	Scene*	GetMinLoadScene();
	Scene*  GetFrontScene();
};

