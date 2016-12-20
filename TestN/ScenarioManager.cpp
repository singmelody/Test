#include "StdAfx.h"
#include "ScenarioManager.h"


ScenarioManager::ScenarioManager(void)
{
}


ScenarioManager::~ScenarioManager(void)
{
}

bool ScenarioManager::LoadDataFromDB(DBInterface* pDBI)
{
	if(!LoadScenarioTemplate(pDBI))
		return false;

	if(!LoadScenarioInfo(pDBI))
		return false;

	return true;
}

bool ScenarioManager::LoadScenarioInfo(DBInterface* pDBI)
{
	
}

bool ScenarioManager::LoadScenarioParam(DBInterface* pDBI)
{

}

bool ScenarioManager::LoadScenarioTemplate(DBInterface* pDBI)
{

}

void ScenarioInfo::RegisterStepInfo(int32 nStepID, ScenarioStepInfo* pStepInfo)
{

}
