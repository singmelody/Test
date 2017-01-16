#include "StdAfx.h"
#include "CltActorPktHandleManager.h"


CltActorPktHandleManager::CltActorPktHandleManager(void)
{
}


CltActorPktHandleManager::~CltActorPktHandleManager(void)
{
}

void CltActorPktHandleManager::PktNode_InitAvatarData(class PacketInitAvatarData* pPkt)
{
	MyClt* pClt = GetClt(pPkt);
	if(!pClt)
		return;

	pClt->InitAvatarData(pPkt);
}

void CltActorPktHandleManager::PktNode_UpdateAvatarData(class PacketUpdateAvatarData* pPkt)
{
	MyClt* pClt = GetClt(pPkt);
	if(!pClt)
		return;

	pClt->UpdateAvatarData(pPkt);
}
