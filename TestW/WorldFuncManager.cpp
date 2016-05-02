#include "StdAfx.h"
#include "WorldFuncManager.h"


WorldFuncManager::WorldFuncManager() 
	:	ModuleFuncManager(WorldSrv::Instance())
		,WorldSrv(WorldSrv::Instance())
		,ContexMgr(WorldAsynContexManager::Instance())
{
}


WorldFuncManager::~WorldFuncManager(void)
{
}
