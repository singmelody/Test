#pragma once

#include "Team.h"
#include "WorldGroup.h"

class WorldTeam : public Team, public WorldGroup, public TeamSlotContainer
{
	DECLARE_FACTORY_ARG0( WorldTeam, -1, new PoolAllocator)
public:
	WorldTeam();
	virtual ~WorldTeam();

	WorldAvatar* GetLeader();

	virtual TeamMember* GetMemberBySlot(int32 nSlotID);

	class WorldTeamManager& TeamMgr;
};