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
	if(!InitialScenarioTemplate(pDBI))
		return false;

	if(!InitialScenarioInfo(pDBI))
		return false;

	return true;
}

void ScenarioInfo::RegisterStepInfo(int32 nStepID, ScenarioStepInfo* pStepInfo)
{
	if( nStepID < 0 || nStepID >= MaxScenarioStep )
		return;

	StepInfos[nStepID] = pStepInfo;
}

ScenarioStepInfo* ScenarioInfo::GetStepInfo(int32 nStepID)
{
	if( nStepID < 0 || nStepID >= MaxScenarioStep )
		return;

	return StepInfos[nStepID];
}

bool ScenarioManager::InitialScenarioInfo(DBInterface* pDBI)
{
	DBTable table;
	if(!pDBI->ExecuteSql( m_sqlDefine.c_str(), &table))
		return false;

	DBRowList& rows = table.m_rowList;

	DBRowList::iterator itr = rows.begin();
	DBRowList::iterator itrEnd = rows.end();

	for (; itr != itrEnd; ++itr)
	{
		if(!(*itr))
			continue;

		DBRow& row = *(*itr);

		ScenarioInfo* pInfo = new ScenarioInfo(row);
		row.Release();

		if(!pInfo)
			continue;

		pInfo->ScenarioTemplateClass = GetTemplateClassName(pInfo->ScenarioTemplateID);

		(*this)[pInfo->nScenarioID] = pInfo;
	}

	if(!InitialScenarioParam(pDBI))
		return false;

	return true;
}

bool ScenarioManager::InitialScenarioParam(DBInterface* pDBI)
{
	DBTable table;
	if(!pDBI->ExecuteSql( m_sqlParam.c_str(), &table))
		return false;

	int32 nCol_ScenarioID = table.GetColumnIdx("ScenarioID");

	ScenarioParam::DBInfo dbinfo(table);

	DBRowList& rows = table.m_rowList;

	DBRowList::iterator itr = rows.begin();
	DBRowList::iterator itrEnd = rows.end();

	for (; itr != itrEnd; ++itr)
	{
		if(!(*itr))
			continue;

		DBRow& row = *(*itr);

		int32 nScenarioID = GetValue<int>( row, nCol_ScenarioID, 0);

		ScenarioInfo* pInfo = (ScenarioInfo*)GetItem(nScenarioID);
		if(!pInfo)
			continue;

		ScenarioParam* pParam = new ScenarioParam( dbinfo, row);
		if(!pParam)
			continue;

		pInfo->AddItem( pParam->ParamName, pParam);
	}

	return true;
}

bool ScenarioManager::InitialScenarioTemplate(DBInterface* pDBI)
{
	DBTable table;
	if(!pDBI->ExecuteSql( m_sqlTemplate.c_str(), table))
		return false;

	DBRowList& rows = table.m_rowList;
	DBRowList::iterator itrEnd = rows.end();

	for (DBRowList::iterator itr = rows.begin(); itr != itrEnd; ++itr)
	{
		if(!*itr)
			continue;

		ScenarioTemplateInfo* pInfo = new ScenarioTemplateInfo(**itr);
		if(!pInfo)
			m_mapTemplates[pInfo->nTemplateID] = pInfo;

		(*itr)->Release();
	}

	return true;
}


std::string ScenarioManager::GetTemplateClassName(int32 nTemplateID)
{
	ScenarioTemplateMap::iterator iter = m_mapTemplates.find( nTemplateID );
	if( iter != m_mapTemplates.end() )
		return iter->second->TemplateClass;

	return m_dftScenarioClass;
}

