#include "StdAfx.h"
#include "AvatarSrvEx.h"


AvatarSrvEx::AvatarSrvEx(void)
{
	m_sceneID = SCENE_ID_NULL;
	m_pCommonDataMgr = NULL;
	m_pCDComponent = NULL;
	
}


AvatarSrvEx::~AvatarSrvEx(void)
{
}

Scene* AvatarSrvEx::GetScene()
{
	return NULL;
}

void AvatarSrvEx::SetScene(Scene* pScene)
{
	if(pScene == NULL)
		m_sceneID = SCENE_ID_NULL;
	else
		m_sceneID = pScene->GetSceneID();
}


