#include "StdAfx.h"
#include "WorldState.h"
#include "WorldAvatar.h"
#include "MyLog.h"
#include "WorldAvatarManager.h"

WorldState::WorldState(void)
{
}


WorldState::~WorldState(void)
{
}

void WorldState::OnEnterState(WorldAvatar* pAvatar)
{
	m_listAvatars.Add(pAvatar->GetTickNode());
	pAvatar->SetPendingTime(Create_Avatar_Pending_Time);
}

void WorldState::OnLeaveState(WorldAvatar* pAvatar)
{
	m_listAvatars.Remove(pAvatar->GetTickNode());
}

bool WorldState::CheckInState(WorldAvatar* pAvatar, const char* pSzErrorLocation)
{
	assert(pSzErrorLocation);

	if(!pAvatar)
		return false;

	if(pAvatar->GetCurState() == this)
		return true;

	CONSOLE_PINK;
	MyLog::error("Avatar Check Stage Failed, avatarid[%d], avatarstage[%d], tarstage[%d] location=[%s]",
		pAvatar->GetAvatarID(), pAvatar->m_nCurStageID, GetState(), pSzErrorLocation);

	return false;
}

void WorldState::DestroyAvatar(WorldAvatar* pAvatar)
{
	AvatarMgr.RemoveWorldAvatar(pAvatar);
}

void WorldState::OnGateClosed(int32 nSrvID)
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

		if(pAvatar->GetGateSrvID() == nSrvID)
			DestroyAvatar(pAvatar);
	}
}

WorldAvatar* WorldState::GetWorldAvatarAndCheckStage(int32 nAvatarID, const char* pSzaErrorLocation)
{
	assert( pSzaErrorLocation );

	WorldAvatar* pAvatar = GetWorldAvatar(nAvatarID);
	if(!pAvatar)
	{
		MyLog::error("GetWorldAvatar Fail! id=[%d] location=[%s]", nAvatarID, pSzaErrorLocation);
		return 0;
	}

	if(!CheckInState( pAvatar, pSzaErrorLocation))
		return 0;

	return pAvatar;
}



