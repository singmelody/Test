#pragma once
#include "NodeScenario.h"
class Scenario_LoadContent : public NodeScenario
{
	DECLARE_FACTORY_ARG0(Scenario_LoadContent, -1, new PoolAllocator);	
public:
	Scenario_LoadContent(void);
	~Scenario_LoadContent(void);

	virtual void InitStepFuncs(ScenarioInfo* pInfo);

	ScenarioStepResult StartStep( ScenarioStepAction action, f32 dt);
	ScenarioStepResult CheckAndLoadContent( ScenarioStepAction action, f32 dt);
protected:
};

