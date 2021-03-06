#pragma once
#include "GameObject.h"
#include "ScenarioDef.h"


#define REG_SCENARIO_STEP( pScenarioInfo, classObj, function, funcID, succeedFuncID) \
	pScenarioInfo->RegisterStepInfo( funcID, new ScenarioStepInfoEx<classObj>( &classObj::function, succeedFuncID, succeedFuncID));

#define REG_SCENARIO_STEP2( pScenarioInfo, classObj, function, funcID, succeedFuncID, failedFuncID) \
	pScenarioInfo->RegisterStepInfo( funcID, new ScenarioStepInfoEx<classObj>( &classObj::function, succeedFuncID, failedFuncID));

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

	virtual void DestroyScenario(bool bNotify = false);

	virtual bool StartScenario();
	virtual bool TickScenario(int32 nFrameInterval);

	bool SetCurStep(int32 nStepID);

	ScenarioFlow*		m_pScenarioFlow;
	ScenarioInfo*		m_pScenarioInfo;
	ScenarioStepInfo*	m_pCurStep;

	f32	m_fTickTime;
	f32 m_fCurTime;
};

