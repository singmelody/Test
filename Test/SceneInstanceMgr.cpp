#include "StdAfx.h"
#include "SceneInstanceMgr.h"
#include "SceneInfo.h"
#include "Scene.h"

SceneInstanceMgr::SceneInstanceMgr(void)
{
}


SceneInstanceMgr::~SceneInstanceMgr(void)
{
}

uint32 SceneInstanceMgr::AllocSceneID(bool bRand)
{
	if (bRand)
	{
		for (int32 i = 0; i < 64; ++i)
		{
			uint16 nRandid = rand();
			if(nRandid == 0)
				continue;

			uint32 nSceneID = SceneInfo::GenSceneID( m_pSceneInfo->m_SceneSID, nRandid);

			if( find(nSceneID) == end() )
				return nSceneID;
		}
	}
	else
	{
		for (int32 i = 1; i < 50; ++i)
		{
			int32 nSceneID = SceneInfo::GenSceneID( m_pSceneInfo->m_SceneSID, i);

			if( find( nSceneID) == end() )
				return nSceneID;
		}
	}

	return SCENE_ID_NULL;
}

Scene* SceneInstanceMgr::GetMinLoadScene()
{
	Scene* pMinScene = NULL;
	for (iterator itr = begin(); itr != end(); ++itr)
	{
		Scene* pScene = itr->second;
		if(!pScene->IsRunning())
			continue;

		if(pMinScene)
		{
			pMinScene = pScene;
			continue;
		}

		if( pScene->GetPlayerCount() < pMinScene->GetPlayerCount() )
		{
			pMinScene = pScene;
			continue;
		}
	}

	return pMinScene;
}

Scene* SceneInstanceMgr::GetFrontScene()
{
	return NULL;
}
