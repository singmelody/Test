#include "StdAfx.h"
#include "WorldTeam.h"
#include "WorldAvatar.h"

WorldTeam::WorldTeam()
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
