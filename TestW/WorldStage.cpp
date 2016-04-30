#include "StdAfx.h"
#include "WorldStage.h"


WorldStage::WorldStage(void)
{
}


WorldStage::~WorldStage(void)
{
}

void WorldStage::OnEnterStage(WorldAvatar* pAvatar)
{
	m_listAvatars.Add(pAvatar->GetTickNode());
	pAvatar->SendPendingTime(Create_Avatar_Pending_Time);
}

void WorldStage::OnLeaveStage(WorldAvatar* pAvatar)
{
	m_listAvatars.Remove(pAvatar->GetTickNode());
}

bool WorldStage::CheckInStage(WorldAvatar* pAvatar, const char* pSzErrorLocation)
{
	assert(pSzErrorLocation);

	if(!pAvatar)
		return false;

	if(pAvatar->m_pCurStage == this)
		return true;

	CONSOLE_PINK;
	MyLog::error("Avatar Check Stage Failed, avatarid[%d], avatarstage[%d], tarstage[%d] location=[%s]",
		pAvatar->GetAvatarID(), pAvatar->m_CurStageID, GetState(), pSzErrorLocation);

	return false;
}

void WorldStage::DestroyAvatar(WorldAvatar* pAvatar)
{
	AvatarMgr::RemoveWorldAvatar(pAvatar);
}

void WorldStage::OnGateClosed(int32 nSrvID)
{
	// gate invalid, login out player
	if(nSrvID == SrvID_NULL)
		return;

	TickList& list = m_listAvatars;

	PTICKNODE pNode = list.GetNext(NULL);
	while( pNode != NULL )
	{
		WorldAvatar* pAvatar = (WorldAvatar*)( pNode->Get() );
		if( !pAvatar )
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);

		if(pAvatar->m_GateSrvID == nSrvID)
			DestroyAvatar(pAvatar);
	}
}

WorldAvatar* WorldStage::GetWorldAvatarAndCheckStage(int32 nAvatarID, const char* pSzaErrorLocation)
{
	assert( pSzaErrorLocation );

	WorldAvatar* pAvatar = GetWorldAvatar(nAvatarID);
	if(!pAvatar)
	{
		MyLog::error("GetWorldAvatar Fail! id=[%d] location=[%s]", avatarid, pSzaErrorLocation);
		return 0;
	}

	if(!CheckInStage( pAvatar, pSzaErrorLocation))
		return 0;

	return pAvatar;
}



