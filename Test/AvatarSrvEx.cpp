#include "StdAfx.h"
#include "AvatarSrvEx.h"
#include "Scene.h"
#include "ParamPool.h"

AvatarSrvEx::AvatarSrvEx(void)
{
	m_sceneID = SCENE_ID_NULL;
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
		m_sceneID = SCENE_ID_NULL;
	else
		m_sceneID = pScene->GetSceneID();
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


