#pragma once

class ScenarioStepInfo
{
public:
	ScenarioStepInfo()
	{
		SucceedFuncID = NULL;
		FailedFuncID = NULL;
	}

	virtual void EnterScenario(Scenario* pScenario) = 0;
	virtual bool TickScenario(Scenario* pScenario, f32 deltaTime) = 0;
	virtual void LeaveScenario(Scenario* pScenario) = 0;
};