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

class ScenarioTemplateMap : public std::map< int32, ScenarioTemplateInfo>
{

};

class ScenarioInfo : public ScenarioParamMap
{
	enum
	{
		eSet_Weekday		= 1 << 0,
		eSet_TimeInterval	= 1 << 1,
	};
public:
	ScenarioInfo(DBRow& row);

	Scenario* CreateScenario();

	bool IsSetWeekday() { return ( ScenarioAttribute & eSet_Weekday) != 0; }
	bool IsSetTimeInterval() { return ( ScenarioAttribute & eSet_TimeInterval) != 0;}

	void RegisterStepInfo( int32 nStepID, ScenarioStepInfo* pStepInfo);
	ScenarioStepInfo* GetStepInfo(int32 nStepID);

	int32				nScenarioID;
	std::string			ScenarioName;
	int32				ScenarioTemplateID;
	int32				ScenarioAttribute;

	std::string			ScenarioTemplateClass;
	int32				nIntParam1;
	int32				nIntParam2;
	f32					fParam1;
	std::string			StrParam;

	bool				bStepFuncsInit;
	ScenarioStepInfo*	StepInfos[MaxScenarioStep];
};

class ScenarioManager : public LoadTemplate, public ID2ItemMap< int32, ScenarioInfo>
{
public:
	ScenarioManager(void);
	~ScenarioManager(void);

	virtual bool LoadDataFromDB(DBInterface* pDBI);

	bool InitialScenarioInfo( DBInterface* pDBI);
	bool InitialScenarioParam( DBInterface* pDBI);
	bool InitialScenarioTemplate( DBInterface* pDBI);

	ScenarioTemplateMap	m_mapTemplates;

	std::string GetTemplateClassName(int32 nTemplateID);

	std::string m_sqlDefine;
	std::string m_sqlParam;
	std::string m_sqlTemplate;

	std::string m_dftScenarioClass;
};

