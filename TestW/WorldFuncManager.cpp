#include "StdAfx.h"
#include "WorldFuncManager.h"
#include "WorldAsynContextManager.h"
#include "WorldServer.h"

WorldFuncManager::WorldFuncManager() 
	:	ModuleFuncManager(WorldServer::Instance())
		,WorldSrv(WorldServer::Instance())
		,ContexMgr(WorldAsynContextManager::Instance())
{
}


WorldFuncManager::~WorldFuncManager(void)
{
}
