#pragma once

#include "Scenario.h"
#include "DBInterface.h"
#include "ID2ItemMap.h"

class ScenarioStepInfo
{
public:
	ScenarioStepInfo()
	{
		nSucceedFuncID = NULL;
		nFailedFuncID = NULL;
	}

	virtual void EnterScenario(Scenario* pScenario) = 0;
	virtual bool TickScenario(Scenario* pScenario, f32 deltaTime) = 0;
	virtual void LeaveScenario(Scenario* pScenario) = 0;

	int32 nSucceedFuncID;
	int32 nFailedFuncID;
};

class ScenarioParam
{
public:
	class DBInfo
	{
	public:
		DBInfo( DBTable& table);
	};

	ScenarioParam( DBInfo dbInfo, DBRow& row);

	std::string ParamName;
	int32 nParamInt1;
	int32 nParamInt2;
	f32 fParamFloat;

	std::string ParamString;
};

class ScenarioParamMap : public ID2ItemMap<std::string, ScenarioParam>
{};