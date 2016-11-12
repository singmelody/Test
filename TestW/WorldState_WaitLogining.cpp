#include "StdAfx.h"
#include "WorldState_WaitLogining.h"
#include "WorldAvatarManager.h"
#include "WorldAvatar.h"

WorldState_WaitLogining::WorldState_WaitLogining(void)
{
}


WorldState_WaitLogining::~WorldState_WaitLogining(void)
{
}

void WorldState_WaitLogining::Tick(int32 nFrameTime)
{
	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);
	while ( pNode != NULL )
	{
		WorldAvatar* pAvatar = (WorldAvatar*)(pNode->Get());
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = pNode->next();

		int32 nAvatarID = -1;
		int32 nRet = AvatarMgr.CheckAccountOnline( pAvatar->Account.GetAccountName().c_str(), nAvatarID);
		if( nRet == 0)
			pAvatar->SetCurState(eWS_Logining);

		int32 nPendTime = pAvatar->GetPendingTime();
		if( nPendTime <= 0)
		{
			AvatarMgr.RemoveWorldAvatar(pAvatar);
			continue;
		}
		pAvatar->SubPendingTime( nFrameTime );
	}
}
