#include "StdAfx.h"
#include "WorldSceneInfo.h"
#include "WorldScene.h"
#include "SceneManager.h"
#include "WorldAvatar.h"
#include "WorldAsynContextManager.h"
#include "WorldSceneManager.h"

SceneRequestList::SceneRequestList()
{

}


WorldSceneInfo::WorldSceneInfo(void)
	: SceneMgr( WorldSceneManager::Instance() )
	, ContextMgr( WorldAsynContextManager::Instance() )
	, EnterMgr( WorldEnterManager::Instance() )
{

}


WorldSceneInfo::~WorldSceneInfo(void)
{
}


bool WorldSceneInfo::LoadSceneInfo(DBRow& row)
{

}

void WorldSceneInfo::OnSceneCreateSucceed(WorldScene* pScene)
{

}

void WorldSceneInfo::OnSceneCreateFailed(WorldScene* pScene, int32 nErrorID)
{

}

bool WorldSceneInfo::TryEnterTargetScene(WorldAvatar* pAvatar, int16 nInstanceID, int32& nFailReason)
{
	if(nInstanceID == SCENE_ID_NULL)
		return false;

	int32 nTargetSceneID = SceneInfo::GenSceneID( m_nSceneSID, nInstanceID);

	WorldScene* pScene = (WorldScene*)m_Instances.GetItem(nTargetSceneID);
	if(!pScene)
	{
		nFailReason = eChangeSceneError_SceneNotExist;
		return false;
	}

	return TryEnterScene( pAvatar, pScene, nFailReason);
}

int32 WorldSceneInfo::AllocSceneID()
{
	return m_Instances.AllocSceneID( !IsMainTrunk() );
}

bool WorldSceneInfo::TryEnterScene(WorldAvatar* pAvatar, WorldScene* pScene, int32& nFailReason)
{
	if(!pScene)
		return false;

	if(!pAvatar)
		return false;

	if( !pScene->IsRunning() && !pScene->IsWaitClosing() )
	{
		nFailReason = eChangeSceneError_SceneIsClosing;
		return false;
	}

	if(pScene->IsPlayersFull())
	{
		nFailReason = eChangeSceneError_SceneIsFull;
		return false;
	}

	if(!pAvatar->CheckPlayerEnterScene(pScene))
	{
		nFailReason = eChangeSceneError_CheckEnterFailed;
		return false;
	}

	if(!pScene->CheckEnterScene(pAvatar))
	{
		nFailReason = eChangeSceneError_CheckEnterFailed;
		return false;
	}

	// 
	return EnterMgr.HandleEnterScene( pAvatar, pScene);
}

