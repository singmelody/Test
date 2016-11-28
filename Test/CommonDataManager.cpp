#include "StdAfx.h"
#include "CommonDataManager.h"
#include "FactoryManagerBase.h"
#include "CommonDataObj.h"
#include "PacketImpl.h"

CommonDataManager::CommonDataManager()
{
	m_pCommonDataOwner = NULL;
	memset( m_pCont, 0, sizeof(m_pCont));
}

CommonDataManager::~CommonDataManager()
{
	for (int32 i = eCommonData_Min + 1; i < eCommonData_Max; ++i)
	{
		CommonDataCont* pCont = m_pCont[i];

		if(!pCont)
			continue;

		FACTORY_DELOBJ(pCont);
		ClearCont((CommonDataType)i);
	}
}

void CommonDataManager::SetOwner(CommonDataOwner* pOwner)
{
	m_pCommonDataOwner = pOwner;
}

void CommonDataManager::AddCont(CommonDataCont* pCont)
{
	if(!pCont)
		return;

	CommonDataType type = pCont->GetType();

	if( type <= eCommonData_Min || type >= eCommonData_Max )
		return;

	m_pCont[type] = pCont;
	pCont->SetOwner(m_pCommonDataOwner);
}

void CommonDataManager::ClearCont(CommonDataType type)
{
	if( type <= eCommonData_Min || type >= eCommonData_Max )
		return;

	m_pCont[type] = NULL;
}

CommonDataCont* CommonDataManager::GetCont(CommonDataType type) const
{
	if( type <= eCommonData_Min || type >= eCommonData_Max )
		return NULL;
	
	return m_pCont[type];
}

CommonDataObject* CommonDataManager::OnRecvCreatePacket(class PacketCommonDataCreate* pPkt)
{
	CommonDataObject* pObj = CreateCommonDataObj(pPkt);
	if(!pObj)
		return NULL;

	pObj->ReadParamPoolData( pPkt, eVCM_Init);

	if( !pObj->InitObject() )
	{
		DeleteCommonDataObj(pObj);
		return NULL;
	}

	OnCreateObj(pObj);
	return pObj;
}

void CommonDataManager::DeleteCommonDataObj(CommonDataObject* pObj, bool bDestroy /*= true*/)
{
	CommonDataCont* pCont = GetCont( (CommonDataType)(pObj->m_nCommonDataType));
	if(!pCont)
		return;

	OnDeleteObj(pObj);
	pObj->NotifyDelete();
	pCont->Remove( pObj->GetIndex(), bDestroy);
}

CommonDataObject* CommonDataManager::OnRecvInitPacket(class PacketCommonDataInit* pPkt)
{
	CommonDataObject* pObj = CreateCommonDataObj(pPkt);
	if(!pObj)
		return NULL;

	pObj->ReadParamPoolData( pPkt, eVCM_Init);

	if( !pObj->InitObject() )
	{
		DeleteCommonDataObj(pObj);
		return NULL;
	}

	pObj->InitObjFlag();
	OnInitObj(pObj);
	return pObj;
}


CommonDataObject* CommonDataManager::OnRecvUpdatePacket(class PacketCommonDataUpdate* pPkt)
{
	CommonDataObject* pObj = GetCommonDataObj(pPkt);
	if(!pObj)
		return NULL;

	OnPreUpdateObj(pObj);

	pObj->ReadParamPoolData( pPkt, eVCM_Update, true);
	pObj->OnUpdateObj();
	OnUpdateObj(pObj);

	return pObj;
}

void CommonDataManager::OnRecvDeletePacket(class PacketCommonDataDelete* pPkt)
{
	CommonDataObject* pObj = GetCommonDataObj(pPkt);
	if(!pObj)
		return;

	DeleteCommonDataObj(pObj);
}

void CommonDataManagerGame::AddObserver(CommonDataType type, CommonDataObserver* pObserver)
{
	if(!pObserver)
		return;

	if( type > eCommonData_Min && type < eCommonData_Max)
		m_vObserverList[type].push_back(pObserver);


}

void CommonDataManagerGame::NotifyOberserver(CommonDataObject* pObj, NOTIFYFUNC pFunc)
{
	int32 nType = pObj->m_nCommonDataType;

	if( nType > eCommonData_Min && nType < eCommonData_Max )
	{
		ObserverList& list = m_vObserverList[nType];

		ObserverList::iterator itr = list.begin();
		ObserverList::iterator itr_end = list.end();

		for (; itr != itr_end; ++itr)
		{
			((*itr)->*pFunc)(pObj);
		}
	}
}
