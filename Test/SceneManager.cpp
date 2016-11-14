#include "StdAfx.h"
#include "SceneManager.h"
#include "ParamDef.h"
#include "Scene.h"

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

void SceneManager::OnSceneCreated(Scene* pScene)
{
	m_nSceneLoadValue += pScene->GetLoadValue();
}

void SceneManager::OnSceneDestroied(Scene* pScene)
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

