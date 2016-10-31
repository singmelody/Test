#include "StdAfx.h"
#include "Scene.h"
#include "SceneInfo.h"

Scene::Scene(void)
	:m_nSceneID(SCENE_ID_NULL)
	,m_nSceneLv(1)
	,m_nAreaID(-1)
{
	
	m_nPlayerCnt = 0;
}


Scene::~Scene(void)
{
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
