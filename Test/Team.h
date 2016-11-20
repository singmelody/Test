#pragma once
#include "ParamPool.h"
#include "ID2ItemMap.h"
#include "ParamPoolOwner.h"

class TeamMember;
class WorldAvatar;

#define INVALID_TEAM_SLOT_INDEX		-1
#define INVALID_TEAM_AVATARDID		0
#define MAX_TEAM_AVATAR_COUNT		25

enum{
	eTeam_Post_Member	= 0,
	eTeam_Post_Assit	= 1,
	eTeam_Post_Leader	= 2,
};

class TeamUtil
{
public:
	static bool SlotValid(int32 nTeamSlot);
};

class TeamSlotContainer
{
public:
	TeamSlotContainer();
	virtual ~TeamSlotContainer();

	virtual TeamMember* GetMemberBySlot(int32 nSlotID) = 0;

	int32 GetSlot(int64 nAvatarDID) const;
	int32 GetFirstEmptySlot() const;
	int32 GetEmptyCount() const;

protected:
	bool	SlotValid(int32 nSlotID) const;

	int64	m_slots[MAX_TEAM_AVATAR_COUNT];
};

class TeamMember : public ParamPoolOwner
{
	DECLARE_FACTORY_ARG0( TeamMember, -1, new PoolAllocator)
public:
	TeamMember();
	virtual ~TeamMember();

	int64 m_nAvatarDID;
	int32 m_nAvatarID;

	int32 m_nSceneID;
};

class Team : public ParamPoolOwner, public ID2ItemMap<int64, TeamMember>
{
	DECLARE_FACTORY_ARG0( Team, -1, new PoolAllocator)
public:
	Team(void);
	virtual ~Team(void);

	void SetTeamID( int64 nID);
	int64 GetTeamID();

	int64 GetLeaderDID() const;
	void SetLeaderDID(int64 nAvatarDID);

	int32 GetMemberCount() const;
};

