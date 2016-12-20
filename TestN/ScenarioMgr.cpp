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

	std::list<int32>::iterator itr = m_vCurTickList.begin();

	if((itr == m_vCurTickList.end()) && (list.GetNext(NULL) != NULL ))
	{
		m_bRefresh = true;
	}

	itr = m_vCurTickList.begin();
	while (itr != m_vCurTickList.end() )
	{
		Scenario* pScenario = GetItem(*itr);
		if(!IsValidMemory(pScenario))
		{
			itr = m_vCurTickList.erase(itr);
			continue;
		}

		if(!pScenario->TickScenario(nFrameTime))
		{
			m_bRefresh = true;
			ReMoveFromRootList(pScenario);
			itr = m_vCurTickList.erase(itr);
			continue;
		}
		++itr;
	}

	if(m_bRefresh)
		DoRefreshTickList();
}

void ScenarioMgr::Destroy()
{
	TickList& list = m_TickList;
	PTICKNODE pNode = list.GetNext(NULL);
	while(pNode != NULL)
	{
		Scenario* pScenario = (Scenario*)pNode->Get();
		pScenario->DestroySceanrio();
		pNode = list.GetNext(pNode);
		FACTORY_DELOBJ(pScenario);
	}

	clear();
}
