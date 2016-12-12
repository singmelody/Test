#pragma once

#include "ID2ItemMap.h"
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

class ScenarioMgr : public SceneObjMgr<Scenario>
{
public:
	ScenarioMgr(void);
	~ScenarioMgr(void);

	void Tick(int32 nFrameTime);
	void TickEx(int32 nFrameTime);

	bool				m_bRefresh;
	std::list<int32>	m_CurTickList;
};

