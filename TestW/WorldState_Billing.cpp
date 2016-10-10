#include "StdAfx.h"
#include "WorldState_Billing.h"
#include "PacketImpl.h"
#include "PacketProcessor.h"
#include "WorldServer.h"
#include "WorldAvatar.h"
#include "WorldAvatarManager.h"

WorldState_Billing::WorldState_Billing(void)
{
}


WorldState_Billing::~WorldState_Billing(void)
{
}

void WorldState_Billing::RegPeerPktHandle(class PacketProcessor* pProc)
{
}

void WorldState_Billing::OnEnterState(WorldAvatar* pAvatar)
{
	WorldState::OnEnterState(pAvatar);

	if(!WorldServer::bUseBilling || pAvatar->m_bIsDestroy)
		pAvatar->SetPendingTime(0);
	else
	{
		pAvatar->NoticeBillingLogout(false);
		pAvatar->SetPendingTime( XIAOTUI_TIME );
	}
}

void WorldState_Billing::Tick(int32 nFrameTime)
{
	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);

	while ( pNode != NULL )
	{
		WorldAvatar* pAvatar = (WorldAvatar*)(pNode->Get());
		if( pAvatar == NULL )
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);

		int32 nPendingTime = pAvatar->GetPendingTime();
		if( nPendingTime <= 0 )
		{
			AvatarMgr.RemoveWorldAvatar(pAvatar);
			continue;
		}
		pAvatar->SubPendingTime(nFrameTime);
	}
}
