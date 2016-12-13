#pragma once

#include <map>
#include <string>
#include "ID2ItemMap.h"
#include "DBInterface.h"
#include "ScenarioDef.h"
#include "LoadTemplate.h"


class ScenarioTemplateInfo
{
public:
	ScenarioTemplateInfo(DBRow& row);

	int32			nTemplateID;
	std::string		TemplateClass;
};

class ScenarioInfo : public ScenarioParamMap
{
public:
	ScenarioInfo(DBRow& row);

	Scenario* CreateScenario();

	int32			nScenarioID;
	std::string		ScenarioName;
	int32			ScenarioTemplateID;
	int32			ScenarioAttribute;

	enum
	{
		eSet_Weekday		= 1 << 0,
		eSet_TimeInterval	= 1 << 1,
	};

	bool IsSetWeekday() { return ( ScenarioAttribute & eSet_Weekday) != 0; }
	bool IsSetTimeInterval() { return ( ScenarioAttribute & eSet_TimeInterval) != 0;}

	std::string ScenarioTemplateClass;
	int32	nIntParam1, nIntParam2;
	f32		fParam1,fParam2;
	std::string StrParam;

	bool bStepFuncsInit;

	void RegisterStepInfo( int32 nStepID, ScenarioStepInfo* pStepInfo);
	ScenarioStepInfo* GetStepInfo(int32 nStepID);
	ScenarioStepInfo* StepInfos[MaxScenarioStep];
};

class ScenarioManager : public LoadTemplate, public ID2ItemMap< int32, ScenarioInfo>
{
public:
	ScenarioManager(void);
	~ScenarioManager(void);

	virtual bool LoadDataFromDB(DBInterface* pDBI);

	bool InitScenarioInfo( DBInterface* pDBI);
	bool InitScenarioParam( DBInterface* pDBI);
	bool InitScenarioTemplate( DBInterface* pDBI);

	ScenarioTemplateMap	m_mapTemplates;

	std::string GetTemplateClassName(int32 nTemplateID);

	std::string m_sqlDefine;
	std::string m_sqlParam;
	std::string m_sqlTemplate;

	std::string m_dftScenarioClass;
};

