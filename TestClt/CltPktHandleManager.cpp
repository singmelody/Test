#include "StdAfx.h"
#include "CltPktHandleManager.h"


CltPktHandleManager::CltPktHandleManager(void)
{
}


CltPktHandleManager::~CltPktHandleManager(void)
{
}

MyClt* CltPktHandleManager::GetClient(PacketBase* pPkt)
{
	if(!m_pMyCltManager)
		return NULL;

	return m_pMyCltManager->GetClt(pPkt);
}

GameObj* CltPktHandleManager::GetGameObj(PacketBase* pPkt)
{

}
