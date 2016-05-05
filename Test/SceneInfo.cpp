#include "StdAfx.h"
#include "SceneInfo.h"
#include "SceneManagerBase.h"

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

SceneInfo::SceneInfo(void)
{
}


SceneInfo::~SceneInfo(void)
{
}

uint16 SceneInfo::GetSceneSID(uint32 nSceneID)
{
	nSceneID >>= 16;
	return uint16(nSceneID);
}

uint16 SceneInfo::GetSceneInstanceID(uint32 nSceneID)
{
	nSceneID &= 0xffff;
	return uint16(nSceneID);
}

const SceneInfo* SceneInfo::GetSceneInfo(uint16 nSceneSID)
{
	return SceneManagerBase::GetSceneInfo( nSceneSID );
}
