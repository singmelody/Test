#include "StdAfx.h"
#include "ScenarioManager.h"


ScenarioTemplateInfo::ScenarioTemplateInfo(DBRow& row)
{
	static const int32 nCol_TemplateID = row.GetColumnIdx("TemplateID");
	static const int32 nCol_TemplateClass = row.GetColumnIdx("TemplateClass");

	row.Fill( nTemplateID, nCol_TemplateID, 0);
	row.Fill( TemplateClass, nCol_TemplateClass, "");

	if(TemplateClass.length() < 5)
		TemplateClass = "NodeScenario";
}

ScenarioInfo::ScenarioInfo(DBRow& row)
{
	static const int32 Col_ScenarioID = row.GetColumnIdx("ScenarioID");
	static const int32 Col_ScenarioName = row.GetColumnIdx("ScenarioName");
	static const int32 Col_ScenarioTemplateID = row.GetColumnIdx("ScenarioTemplateID");

	static const int32 Col_IntParam1 = row.GetColumnIdx("IntParam1");
	static const int32 Col_IntParam2 = row.GetColumnIdx("IntParam2");
	static const int32 Col_FloatParam1 = row.GetColumnIdx("FloatParam1");
	static const int32 Col_StrParam = row.GetColumnIdx("StrParam");

	row.Fill( nScenarioID, Col_ScenarioID, 0);
	row.Fill( ScenarioName, Col_ScenarioName, "");
	row.Fill( ScenarioTemplateID, Col_ScenarioTemplateID, 0);

	row.Fill( nIntParam1, Col_IntParam1, 0);
	row.Fill( nIntParam2, Col_IntParam2, 0);
	row.Fill( fParam1, Col_FloatParam1, 0.0f);
	row.Fill( StrParam, Col_StrParam, "");

	bStepFuncsInit = false;
	memset( StepInfos, 0, sizeof(ScenarioStepInfo*) * MaxScenarioStep);
}

Scenario* ScenarioInfo::CreateScenario()
{
	Scenario* pScenario = (Scenario*)FactoryManager::Instance().New( ScenarioTemplateClass.c_str() );
	if(!pScenario)
		return;

	pScenario->m_pScenarioInfo = this;
	if(!bStepFuncsInit)
	{
		bStepFuncsInit = true;
		pScenario->InitStepFuncs( this );
	}

	return pScenario;
}

void ScenarioInfo::RegisterStepInfo(int32 nStepID, ScenarioStepInfo* pStepInfo)
{

}

ScenarioManager::ScenarioManager(void)
{
}


ScenarioManager::~ScenarioManager(void)
{
}

bool ScenarioManager::InitialDataFromDB(DBInterface* pDBI)
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

