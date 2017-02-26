#include "StdAfx.h"
#include "GameScene.h"


GameScene::GameScene(void)
{
	m_nSceneID = 0;

	m_nSceneSizeX = 2000;
	m_nSceneSizeY = 2000;
	m_nSceneSizeZ = 100;

	m_bDrawDirty = true;

	m_nZoneID = -1;

	m_pLocalPlayer = NULL;
	m_pSelectActor = NULL;
}


GameScene::~GameScene(void)
{
}

void GameScene::Init(int32 nSceneID, int32 nZoneID)
{
	m_nSceneID = nSceneID;
	m_nSceneSID = SceneInfo::GetSceneSID(nSceneID);

	SceneInfo* pSI = (SceneInfo*)SceneInfo::GetSceneInfo(m_nSceneSID);
	if( pSI != NULL )
	{
		m_pSceneInfo = pSI;
		m_nSceneSizeX = pSI->m_nSceneSizeX;
		m_nSceneSizeY = pSI->m_nSceneSizeY;
	}
}

void GameScene::Tick(int32 nFrameTime)
{
	for (GameObjMap::iterator itr = m_mapGameActors.begin(); itr != m_mapGameActors.end(); ++itr)
	{
		GameObj* pObj = itr->second;
		if(!pObj)
			continue;

		pObj->Tick(nFrameTime);
	}
}

CltGameActor* GameScene::GetGameActor(int32 nAvatarID)
{
	GameObjMap::iterator itr = m_mapGameActor.find(nAvatarID);
	if( itr == m_mapGameActor.end() )
		return NULL;

	return (CltGameActor*)itr->second;
}

GameObj* GameScene::GetGameObj(int32 nAvatarID)
{
	CltGameActor* pGameActor = GetGameActor(nAvatarID);
	if(pGameActor != NULL)
		return pGameActor;

	GameDummy* pGameDummy = GetGameDummy(nAvatarID);
	if(pGameDummy != NULL)
		return pGameDummy;

	return NULL;
}


GameDummy* GameScene::GetGameDummy(int32 nAvatarID)
{
	auto itr = m_mapGameActor,find(nAvatarID);
	if( itr == m_mapGameActor.end() )
		return NULL;

	return (GameDummy*)itr->second;
}