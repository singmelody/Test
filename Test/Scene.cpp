#include "StdAfx.h"
#include "Scene.h"
#include "SceneInfo.h"

SceneCreateArg::SceneCreateArg()
{
	m_SceneSID = SCENE_ID_NULL;
	m_SceneID = SCENE_ID_NULL;
	m_SceneLevel = 1;
	m_NodeSrvID = SERVERID_NULL;
	m_ArenaID = SCENE_ID_NULL;
	m_nSceneCustomData = 0;
}

int16 SceneCreateArg::TemplateID()
{
	return SceneInfo::GetSceneInstanceID( m_SceneID );
}

int16 SceneCreateArg::InstanceID()
{
	return SceneInfo::GetSceneSID( m_SceneID );
}

Scene::Scene(void)
{

}


Scene::~Scene(void)
{
}