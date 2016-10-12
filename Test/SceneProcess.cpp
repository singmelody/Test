#include "StdAfx.h"
#include "SceneProcess.h"
#include "ParamPool.h"

SceneProcess::SceneProcess(void)
{
}


SceneProcess::~SceneProcess(void)
{
}

void SceneProcess::SetScenePermanent(ScenePermanentDefine bit)
{
	ParamPool* pPool = GetParamPool();
	if(!pPool)
		return;

	int32 nValue = PARAM_GET_VALUE( pPool, permanent, int32(0));
	nValue |= bit;
	PARAM_SET_VALUE( pPool, permanent, nValue, true);
}

bool SceneProcess::CheckScenePermanent(ScenePermanentDefine bit)
{
	ParamPool* pPool = GetParamPool();
	if(!pPool)
		return false;

	int32 nValue = PARAM_GET_VALUE( pPool, permanent, int32(0));
	return ( nValue & bit ) == bit;
}

void SceneProcess::ClearScenePermanent(ScenePermanentDefine bit)
{
	ParamPool* pPool = GetParamPool();
	if(!pPool)
		return;

	int32 nValue = PARAM_GET_VALUE( pPool, permanent, int32(0));
	nValue &= ~bit;
	PARAM_SET_VALUE( pPool, permanent, nValue, true);

}
