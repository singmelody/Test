#pragma once
#include "GameObject.h"
#include "ScenarioDef.h"

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

	bool SetCurStep(int32 nStepID);

	ScenarioStepInfo* m_pCurStep;

	f32	m_fTickTime;
	f32 m_fCurTime;
};

