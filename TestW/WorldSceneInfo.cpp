#include "StdAfx.h"
#include "WorldSceneInfo.h"


WorldSceneInfo::WorldSceneInfo(void)
	: EnterMgr( WorldEnterManager::Instance() )
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

	if(!pAvatar->CheckEnterScene(pScene))
	{
		nFailReason = eChangeSceneError_CheckEnterFailed;
		return false;
	}

	// 
	return EnterMgr::HandleEnterScene();
}

void WorldSceneInfo_Trunk::OnSceneCreateSucceed(WorldScene* pScene)
{

}

void WorldSceneInfo_Trunk::OnSceneCreateFailed(WorldScene* pScene)
{

}
