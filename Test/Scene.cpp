#include "StdAfx.h"
#include "Scene.h"
#include "SceneInfo.h"
#include "ConfigManager.h"
#include "ParamPool.h"

Scene::Scene(void)
	:m_nSceneID(SCENE_ID_NULL)
	,m_nSceneLv(1)
	,m_nArenaID(-1)
{
	
	m_nPlayerCnt = 0;
}


Scene::~Scene(void)
{
}

bool Scene::Tick(int32 nDeltaTime)
{
	if( m_pParamPool )
	{
		// 场景计时可以放在这里
	}

	return true;
}

bool Scene::IsPlayersCrowded()
{
	int32 nCrowdedCnt = m_pSceneInfo->m_nPlayerMax * ConfigManager::MainTrunkCrowdedFactor;
	if( nCrowdedCnt <= 0 )
		nCrowdedCnt = 1;

	return m_nPlayerCnt >= nCrowdedCnt;
}

int32 Scene::GetLoadValue()
{
	f32 loadValue = m_pSceneInfo->m_nLoadValue;

	if( m_pSceneInfo->IsMainTrunk() )
	{
		int32 nInstanceID = InstanceID();
		switch(nInstanceID)
		{
		case 1:
			loadValue *= 2.0f;
			break;
		case 2:
			loadValue *= 1.5f;
			break;
		}
	}

	return loadValue;
}

bool Scene::SetProcessBits(int64 nProcessBits)
{
	if( m_nSceneProcessBits == nProcessBits ) 
		return false;

	m_nSceneProcessBits = nProcessBits;

	if( m_pParamPool )
		PARAM_SET_VALUE( m_pParamPool, SceneProcessBits, nProcessBits, true);

	return true;
}
