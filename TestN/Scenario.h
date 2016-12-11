#pragma once
#include "GameObject.h"

class ScenarioInfo;
class ScenarioFlow;

class Scenario : public GameObject
{
public:
	Scenario(void);
	virtual ~Scenario(void);

	virtual void InitStepFuncs(ScenarioInfo* pInfo){}

	void ParseScenarioParams( ScenarioParamMap* pMap);
	virtual void OnParseParam(ScenarioParam& param);

	virtual bool TickScenario(int32 nFrameInterval);

	ScenarioStepInfo* m_pCurStep;
};

