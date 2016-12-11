#include "StdAfx.h"
#include "ScenarioMgr.h"
#include <list>

ScenarioMgr::ScenarioMgr(void)
{
}


ScenarioMgr::~ScenarioMgr(void)
{
}

void ScenarioMgr::Tick(int32 nFrameTime)
{

}

void ScenarioMgr::TickEx(int32 nFrameTime)
{
	TickList& list = m_TickList;

	std::list<int32>::iterator itr = m_CurTickList.begin();

	if((itr == m_CurTickList.end()) && (list.GetNext(NULL) != NULL ))
	{
		m_bRefresh = true;
	}

	itr = m_CurTickList.begin();
	while (itr != m_CurTickList.end() )
	{
		Scenario* pScenario = GetItem(*itr);
		if(!IsValidMemory(pScenario))
		{
			itr = m_CurTickList.erase(itr);
			continue;
		}

		if(!pScenario->TickScenario(nFrameTime))
		{
			m_bRefresh = true;
			ReMoveFromRootList(pScenario);
			itr = m_CurTickList.erase(itr);
			continue;
		}
		++itr;
	}

	if(m_bRefresh)
		DoRefreshTickList();
}
