#include "StdAfx.h"
#include "WorldFuncManager.h"
#include "WorldAsynContextManager.h"


WorldFuncManager::WorldFuncManager() 
	:	ModuleFuncManager(WorldSrv::Instance())
		,WorldSrv(WorldSrv::Instance())
		,ContexMgr(WorldAsynContextManager::Instance())
{
}


WorldFuncManager::~WorldFuncManager(void)
{
}

void WorldFuncManager::SubWorld2WarWorld(class PacketBase* pPkt)
{
	ServerInfo* pInfo = Servers.GetWarWorldInfo();
	if(!pInfo)
		return;

	PeerSend( pPkt, pInfo->nSocketID);
}
