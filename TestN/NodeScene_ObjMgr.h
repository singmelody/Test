#pragma once


#include "Scene.h"
#include "ISceneObjFinder.h"
#include "Scenario.h"


template <class T>
class SceneObjMgr : public ID2ItemMap<int32, T>
{
public:
	SceneObjMgr()
	{
		m_nIDCounter = -10;
	}

	int32 m_nIDCounter;
};

class NodeScene_ObjMgr : public Scene, public ISceneObjFinder
{
public:
	NodeScene_ObjMgr(void);
	virtual ~NodeScene_ObjMgr(void);
};