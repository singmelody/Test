#include "StdAfx.h"
#include "WorldEnterManager.h"


WorldEnterManager::WorldEnterManager(void)
{
}


WorldEnterManager::~WorldEnterManager(void)
{
}

bool WorldEnterManager::HandleEnterScene(WorldAvatar* pAvatar, WorldScene* pScene)
{
	if(!pScene)
		return false;

	int32 nTarNodeID = pScene->GetNodeID();
	int32 nTarSceneID = pScene->m_SceneID;

	WorldSceneInfo* pInfo = (WorldSceneInfo*)( pScene->GetSceneInfo() );
	if(!pInfo)
		return false;

	assert( nTarNodeID != SrvID_NULL );

	Vector3& pos = pAvatar->GetTargetScenePoint();
	Vector3& dir = pAvatar->GetTargetSceneDir();

	WorldScene* pOldScene = (WorldScene*)pAvatar->GetScene();

	if( pOldScene )
	{
	}
	
	pAvatar->m_bTargetNodeAvatarCreated = false;
	return;

}
