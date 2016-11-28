#include "StdAfx.h"
#include "CommonDataObj.h"
#include "ParamDefManager.h"
#include "CommonDataOwner.h"
#include "CommonDataManager.h"
#include "ParamPool.h"
#include "MyLog.h"

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

	m_nCommonDataType = -1;
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
	if(!m_pCommonDataOwner)
		return;

	CommonDataManager* pMgr = m_pCommonDataOwner->GetCommonDataManager();
	if(pMgr)
		pMgr->NotifyDelete(this);
}

void CommonDataObject::ReadParamPoolData(class PacketParamPool* pPkt, int32 nVcMode, bool bDirty /*= false*/)
{
	if(!pPkt)
		return;

	m_nValueChangeMode = nVcMode;

	char tmpBuffer[PacketParamPool::MAX_PACKET_BUFFER_SIZE] = {0};

	int32 nBuffLen = pPkt->ReadBuffer( tmpBuffer, sizeof(tmpBuffer));

	m_pParamPool->Read( tmpBuffer, bDirty);

	m_nValueChangeMode = eVCM_None;
}

void CommonDataObject::SetIndex(int32 nIdx)
{
	m_nIdx = nIdx;

	static bool s_bIsNodeAndSHMEnable = ParamDefManager::Instance().IsNodeServer() && ParamDefManager::Instance().IsSHMEnable();
	if(!s_bIsNodeAndSHMEnable)
		return;

	ParamPool* pPool = GetParamPool();
	if(pPool)
		pPool->NodeServer_WriteSHM_CommonDataIdx(nIdx);
	else
		MyLog::message("CommonDataObject::SetIndex No ParamPool");
}

