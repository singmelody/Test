#include "StdAfx.h"
#include "SceneManager.h"
#include "ParamDef.h"
#include "Scene.h"
#include "MyLog.h"

SceneManager::SceneManager(void)
{
	m_nSceneLoadValue = 0;
}


SceneManager::~SceneManager(void)
{
}


SceneInstanceMgr* SceneManager::GetSceneMap(uint16 nSceneSID)
{
	SceneInfoEx* pSD = (SceneInfoEx*)GetSceneInfo(nSceneSID);
	if( pSD == NULL )
		return NULL;

	return &pSD->m_Instances;
}

Scene* SceneManager::CreateScene(SceneCreateArg& arg)
{
	SceneInfoEx* pSI = (SceneInfoEx*)GetSceneInfo(arg.m_nSceneSID);
	if(!pSI)
		return NULL;

	SceneInstanceMgr* pMgr = GetSceneMap( arg.m_nSceneSID );
	if(!pMgr)
		return NULL;

	Scene* pScene = pSI->CreateSceneObj();
	if(pScene)
		return NULL;

	pScene->m_nNodeID = arg.m_NodeSrvID;
	pScene->m_nSceneID = arg.m_SceneID;
	pScene->SetSceneLevel( arg.m_SceneLevel );
	pScene->SetArenaID(arg.m_ArenaID);
	pScene->CreateParamPool( eParam_Scene, arg.m_nSceneSID);

	pScene->BindParamPool();

	if (!pMgr->InitScene(pScene))
	{
		FACTORY_DELOBJ(pScene);
		return NULL;
	}

	(*pMgr)[pScene->m_nSceneID] = pScene;

	pScene->OnCreate(arg);
	pSI->OnSceneCreate(pScene);
	OnSceneCreated(pScene);

	return pScene;
}

bool SceneManager::DestroyScene(int32 nSceneID)
{
	uint16 nSceneSID = SceneInfo::GetSceneSID( nSceneID);

	SceneInfoEx* pSI = (SceneInfoEx*)GetSceneInfo(nSceneSID);
	if(!pSI)
		return false;

	SceneInstanceMgr* pMap = GetSceneMap( nSceneSID );
	if(!pMap)
	{
		MyLog::error("Warning Scene[%d, %d] Destroy Not Exist", nSceneSID, nSceneID);
		return false;
	}

	SceneInstanceMgr::iterator itr = pMap->find(nSceneID);
	if( itr == pMap->end() )
	{
		MyLog::error("Warning Scene[%d, %d] Destroy Not Exist", nSceneSID, nSceneID);
		return false;
	}

	MyLog::message("Scene Manager Free Scene [%d, %d] Memory", nSceneSID, nSceneID);

	Scene* pScene = itr->second;
	if(!pScene)
	{
		pMap->erase(itr);
		return false;
	}

	pMap->erase(itr);
	pSI->OnSceneDestroy(pScene);
	OnSceneDestroy(pScene);

	FACTORY_DELOBJ(pScene);

	return true;
}

void SceneManager::OnSceneCreated(Scene* pScene)
{
	m_nSceneLoadValue += pScene->GetLoadValue();
}

void SceneManager::OnSceneDestroy(Scene* pScene)
{
	m_nSceneLoadValue -= pScene->GetLoadValue();
}

Scene* SceneManager::GetScene(int32 nSceneID)
{
	int16 scenesid = SceneInfo::GetSceneSID(nSceneID);

	SceneInstanceMgr* pMgr = GetSceneMap(scenesid);
	if(!pMgr)
		return NULL;

	SceneInstanceMgr::iterator siter = pMgr->find(nSceneID);
	if( siter == pMgr->end() )
		return NULL;

	return siter->second;
}

void SceneManager::SumSceneCount()
{
	SceneDataMap& map = SceneInfos;

	m_nTrunkCnt = 0;
	m_nCopyCnt = 0;
	m_nTeamCopyCnt = 0;
	m_nScenarioCopyCnt = 0;
	m_nGuildCopyCnt = 0;
	m_nTokenCopyCnt = 0;
	m_nTrunkCopyCnt = 0;
	m_nRootCopyCnt = 0;


	SceneDataMap::iterator lastItr = map.end();

	for (SceneDataMap::iterator itr = map.begin(); itr != lastItr; ++itr)
	{
		SceneInfoEx* pInfo = (SceneInfoEx*)itr->second;

		int32 nCount = (int32)pInfo->m_Instances.size();

		switch( pInfo->m_CreateType )
		{
		case SceneInfo::eSceneCreate_MainTrunk:
		case SceneInfo::eSceneCreate_WarTrunk:
		case SceneInfo::eSceneCreate_GuildWar:
			m_nTrunkCnt += nCount;
			break;

		case SceneInfo::eSceneCreate_TeamCopy:
			m_nTeamCopyCnt += nCount;
			break;

		case SceneInfo::eSceneCreate_ScenarioCopy:
			m_nSceneCnt += nCount;
			break;

		case SceneInfo::eSceneCreate_GuildCopy:
			m_nGuildCopyCnt += nCount;
			break;
		
		case SceneInfo::eSceneCreate_TokenCopy:
			m_nTokenCopyCnt += nCount;
			break;

		case SceneInfo::eSceneCreate_TrunkCopy:
			m_nTrunkCopyCnt += nCount;
			break;

		case SceneInfo::eSceneCreate_RootCopy:
			m_nRootCopyCnt += nCount;
			break;
		}
	}

	m_nCopyCnt	= m_nTeamCopyCnt + m_nSceneCnt + m_nGuildCopyCnt + m_nTokenCopyCnt + m_nTrunkCopyCnt + m_nRootCopyCnt;
	m_nSceneCnt = m_nTrunkCnt + m_nCopyCnt;
 }