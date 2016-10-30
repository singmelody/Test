#include "StdAfx.h"
#include "SceneInstanceMgr.h"


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

			uint32 nSceneID = SceneInfo::GenSceneID( m_pSceneInfo->m_nSceneID, nRandid);

			if( find(nSceneID) == end() )
				return nSceneID;
		}
	}
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
}
