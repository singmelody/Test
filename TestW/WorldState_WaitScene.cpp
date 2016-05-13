#include "StdAfx.h"
#include "WorldState_WaitScene.h"


WorldStateWaitScene::WorldStateWaitScene(void)
{
}


WorldStateWaitScene::~WorldStateWaitScene(void)
{
}

void WorldStateWaitScene::Tick(int32 nFrameTime)
{
	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);

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
			PacketCltSelectAvatarFail tmpPkt;
			tmpPkt.reason = PacketCltSelectAvatar::eReason_EnterSceneFailed;
			pAvatar->Send2Gate(&tmpPkt, true);

			AvatarMgr.RemoveWorldAvatar(pAvatar);
			continue;
		}

		pAvatar->SubPendingTime(nFrameTime);
	}
}
