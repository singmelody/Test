#pragma once

#include "Team.h"
#include "WorldGroup.h"

class WorldTeam : public Team, public WorldGroup, public TeamSlotContainer
{
public:
	WorldTeam();
	virtual ~WorldTeam();

	WorldAvatar* GetLeader();

	virtual TeamMember* GetMemberBySlot(int32 nSlotID);
};