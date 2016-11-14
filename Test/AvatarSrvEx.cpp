#include "StdAfx.h"
#include "AvatarSrvEx.h"
#include "Scene.h"
#include "ParamPool.h"

AvatarSrvEx::AvatarSrvEx(void)
{
	m_nSceneID = SCENE_ID_NULL;
// 	m_pCommonDataMgr = NULL;
// 	m_pCDComponent = NULL;
	
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
		m_nSceneID = SCENE_ID_NULL;
	else
		m_nSceneID = pScene->GetSceneID();
}

int64 AvatarSrvEx::GetGuildID()
{
	if(!m_pParamPool)
		return INVALID_GUILD_ID;

	return PARAM_GET_VALUE( m_pParamPool, guildid, INVALID_GUILD_ID);
}

void AvatarSrvEx::SetGuildID(int64 nGuildID)
{
	if(!m_pParamPool)
		return;

	return PARAM_SET_VALUE( m_pParamPool, guildid, nGuildID, true);
}

int16 AvatarSrvEx::GetSceneSID()
{
	if( m_nSceneID == SCENE_ID_NULL )
		return SCENE_ID_NULL;

	return SceneInfo::GetSceneSID(m_nSceneID);
}

int16 AvatarSrvEx::GetSceneInstanceID()
{
	if(m_nSceneID == SCENE_ID_NULL )
		return SCENE_ID_NULL;

	return SceneInfo::GetSceneInstanceID(m_nSceneID);
}

int64 AvatarSrvEx::GetTeamID()
{
	if(!m_pParamPool)
		return INVALID_TEAM_ID;

	int64 nTeamID = PARAM_GET_VALUE( m_pParamPool, teamid, INVALID_TEAM_ID);
	return nTeamID;
}

void AvatarSrvEx::SetTeamID(int64 nID)
{
	if(!m_pParamPool)
		return;
	
	PARAM_SET_VALUE( m_pParamPool, teamid, nID, true);
}

const char* AvatarSrvEx::GetName()
{
	if(!m_pParamPool)
		return "";

	return PARAM_GET_STRING( m_pParamPool, title, "");
}

const char* AvatarSrvEx::GetAccountName()
{
	if(!m_pParamPool)
		return "";

	return PARAM_GET_STRING( m_pParamPool, accountname, "");
}

void AvatarSrvEx::ReleaseCommonDataConts()
{
	if(!m_pCommonDataMgr)
		return;

	for (int32 nType = eCommonData_Min + 1; nType < eCommonData_Max; ++nType)
		ReleaseCommonDataConts((CommonDataType)nType);

	FACTORY_DELOBJ(m_pCommonDataMgr);
}

void AvatarSrvEx::ReleaseCommonDataConts(CommonDataType type)
{
	CommonDataCont* pCont = GetCommonDataCont(type);
	if(!pCont)
		return;

	FACTORY_DELOBJ(pCont);
	m_pCommonDataMgr->ClearCont(type);
}


