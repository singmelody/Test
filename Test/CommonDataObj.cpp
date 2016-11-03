#include "StdAfx.h"
#include "CommonDataObj.h"
#include "ParamDefManager.h"
#include "CommonDataOwner.h"
#include "CommonDataManager.h"

CommonDataObject::CommonDataObject()
	: m_nIdx(-1), m_nFlag(eObject_NodeTick|eObject_WorldTick)
{
	m_pCommonDataOwner = NULL;
	_next = NULL;
	_prev = NULL;
	pList = NULL;

	m_bSyncCltSelf = false;
	m_bSyncCltAll = false;

	m_bInit = false;
	m_nValueChangeMode = eVCM_None;
}

void CommonDataObject::SetObjectFlag(int32 nFlag)
{
	m_nFlag |= nFlag;

	static bool s_bIsNodeAndSHMEnable = ParamDefManager::Instance().IsNodeServer() && ParamDefManager::Instance().IsSHMEnable();
}

bool CommonDataObject::InitObject()
{
	m_bInit = true;
	return true;
}

void CommonDataObject::InitObjFlag()
{
	SetObjectFlag( eObjectFlag_CltFlag | eObjectFlag_SaveFlag );
}

void CommonDataObject::NotifyCreate(int32 nFlag)
{
	if(!m_pCommonDataOwner)
		return;

	CommonDataManager* pMgr = m_pCommonDataOwner->GetCommonDataManager();
	if( !pMgr )
		pMgr->NotifyCreate( this, nFlag);
}

void CommonDataObject::NotifyDelete()
{

}

