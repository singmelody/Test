#pragma once


#include "Scene.h"
#include "ISceneObjFinder.h"

class NodeScene_ObjMgr : public Scene, public ISceneObjFinder
{
public:
	NodeScene_ObjMgr(void);
	virtual ~NodeScene_ObjMgr(void);
};

class ScenarioMgr : public SceneObjMgr<Scenario>
{
public:
	ScenarioMgr()
	{
		m_bRefresh = false;
		m_nIDCounter = 0;
	}

	TickList m_TickList;

	void Add(Scenario* pScenario);
	void Tick(int32 nFrameTime);
	void TickEx(int32 nFrameTime);

	void Destroy();

	void RefreshTickList() { m_bRefresh = true;}

	bool m_bRefresh;
	std::list<int32>	m_vCurTickList;
	std::set<int64>		finishProcessBits;
	std::set<int64>		finishInstanceBits;
	std::set<int32>		runningList;
};