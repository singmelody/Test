#include "StdAfx.h"
#include "WorldTeam.h"
#include "WorldAvatar.h"
#include "WorldTeamManager.h"

FINISH_FACTORY_ARG0(WorldTeam)
WorldTeam::WorldTeam()
: TeamMgr(WorldTeamManager::Instance())
{

}
  
WorldTeam::~WorldTeam()
{

}

WorldAvatar* WorldTeam::GetLeader()
{
	int64 nLeaderDID = GetLeaderDID();
	return GetWorldAvatarByDID(nLeaderDID);
}

TeamMember* WorldTeam::GetMemberBySlot(int32 nSlotID)
{
	if(!SlotValid(nSlotID))
		return NULL;

	int64 nAvatarDID = m_slots[nSlotID];

	if(nAvatarDID == INVALID_TEAM_AVATARDID)
		return NULL;

	return GetItem(nAvatarDID);
}
