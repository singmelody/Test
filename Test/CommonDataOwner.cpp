#include "StdAfx.h"
#include "CommonDataOwner.h"


CommonDataOwner::CommonDataOwner(void)
{
}


CommonDataOwner::~CommonDataOwner(void)
{
}

CommonDataCont* CommonDataOwner::GetCommonDataCont(CommonDataType type)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->GetCont( type );
}

CommonDataObject* CommonDataOwner::GetCommonDataObj(PacketCommonDataBase* pPkt)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->GetCommonDataObj(pPkt);
}

CommonDataObject* CommonDataOwner::GetCommonDataObj(CommonDataType type, int32 index /*= -1*/)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->GetCommonDataObj( type, index);
}

CommonDataObject* CommonDataOwner::CreateCommonDataObj(PacketCommonDataBase* pPkt)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->CreateCommonDataObj(pPkt);
}

CommonDataObject* CommonDataOwner::CreateCommonDataObj(CommonDataType type, int32 nDataID /*= 0*/, int32 index /*= -1*/)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->CreateCommonDataObj( type, nDataID, index);
}

void CommonDataOwner::DeleteCommonDataObj(CommonDataObject* pObj, bool bDestroy /*= true*/)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return;

	return pMgr->DeleteCommonDataObj( pObj, bDestroy);
}


CommonDataObject* CommonDataOwner::OnRecvUpdatePacket(PacketCommonDataUpdate* pPkt)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->OnRecvUpdatePacket( pPkt);
}

CommonDataObject* CommonDataOwner::OnRecvCreatePacket(PacketCommonDataCreate* pPkt)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->OnRecvCreatePacket( pPkt);
}

CommonDataObject* CommonDataOwner::OnRecvInitPacket(PacketCommonDataInit* pPkt)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return NULL;

	return pMgr->OnRecvInitPacket( pPkt);
}

void CommonDataOwner::OnRecvDeletePacket(PacketCommonDataDelete* pPkt)
{
	CommonDataManager* pMgr = GetCommonDataManager();
	if(!pMgr)
		return;

	return pMgr->OnRecvDeletePacket( pPkt);
}

