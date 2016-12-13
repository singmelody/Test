#include "StdAfx.h"
#include "Scenario_LoadContent.h"


Scenario_LoadContent::Scenario_LoadContent(void)
{
}


Scenario_LoadContent::~Scenario_LoadContent(void)
{
}

void Scenario_LoadContent::InitStepFuncs(ScenarioInfo* pInfo)
{
	REG_SCENARIO_STEP( pInfo, Scenario_LoadContent, StartStep, 0, 1);
	REG_SCENARIO_STEP( pInfo, Scenario_LoadContent, CheckAndLoadContent, 1, 0);
}

ScenarioStepResult Scenario_LoadContent::StartStep(ScenarioStepAction action, f32 dt)
{

}

ScenarioStepResult Scenario_LoadContent::CheckAndLoadContent(ScenarioStepAction action, f32 dt)
{

}
