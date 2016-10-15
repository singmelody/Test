#include "StdAfx.h"
#include "WorldState_ExitGame.h"


WorldState_ExitGame::WorldState_ExitGame(void)
{
}


WorldState_ExitGame::~WorldState_ExitGame(void)
{
}

void WorldState_ExitGame::OnEnterState(WorldAvatar* pAvatar)
{
	WorldStateGame::OnEnterState(pAvatar);

	if(pAvatar->m_bIsDestroy)
	{
		// normal exit
	}
	else
	{
		// to the select scene
		PacketCltLogout pkt;
		pAvatar->SendPacket(&pkt);
	}
}

void WorldState_ExitGame::Tick(int32 nFrameTime)
{

}

void WorldState_ExitGame::OnPullAvatarDataFinish(WorldAvatar* pAvatar)
{

}

void WorldState_ExitGame::DestroyAvatar(WorldAvatar* pAvatar)
{

}

void WorldState_ExitGame::OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData)
{

}
