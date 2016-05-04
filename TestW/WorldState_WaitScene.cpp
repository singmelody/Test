#include "StdAfx.h"
#include "WorldState_WaitScene.h"
#include "GameObject.h"

WorldState_WaitScene::WorldState_WaitScene(void)
{
}


WorldState_WaitScene::~WorldState_WaitScene(void)
{
}

void WorldState_WaitScene::Tick(int32 nFrameTime)
{
	TickList& list = m_listAvatars;

	TickNode* pNode = list.GetNext(NULL);
	while (pNode != NULL)
	{
		WorldAvatar* pAvatar = (WorldAvatar*)(pNode->Get());
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);

		int32 nPendTime = pAvatar->GetPendingTime();
		if( nPendTime <= 0 )
		{
			PacketCltSelectAvatarFailed tmpPkt;
			tmpPkt.reason = PacketCltSelectAvatarFailed::eReason_EnterSceneFailed;
			pAvatar->Send2Gate(&tmpPkt, true);

			AvatarMgr.RemoveWorldAvatar(pAvatar);
			continue;
		}

		pAvatar->SubPendingTime(nFrameTime);
	}
}