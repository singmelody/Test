#include "StdAfx.h"
#include "WorldFuncExManager.h"


WorldFuncExManager::WorldFuncExManager(void)
	: AvatarMgr(WorldAvatarManager::Instance()), SceneMgr( WorldSceneManager::Instance() )
{
}


WorldFuncExManager::~WorldFuncExManager(void)
{
}

void WorldFuncExManager::Send2WarWorld(PacketBase* pPkt)
{
	WorldSrv.SubWorld2WarWorld(pPkt);
}
