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

void WorldSceneInfo_Trunk::OnSceneCreateSucceed(WorldScene* pScene)
{

}

void WorldSceneInfo_Trunk::OnSceneCreateFailed(WorldScene* pScene)
{

}
