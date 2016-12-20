#pragma once

#include "ID2ItemMap.h"
#include "Scenario.h"
#include "NodeScene_ObjMgr.h"
#include <set>

class ScenarioMgr : public SceneObjMgr<Scenario>
{
public:
	ScenarioMgr()
	{
		m_bRefresh = false;
		m_nIDCounter = 0;
	}

	~ScenarioMgr(void);


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

