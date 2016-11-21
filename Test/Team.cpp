#include "StdAfx.h"
#include "Team.h"

FINISH_FACTORY_ARG0(TeamMember);
TeamMember::TeamMember()
{
	m_nAvatarDID = 0;
	m_nAvatarID = 0;

	m_nSceneID = 0;
}

bool TeamUtil::SlotValid(int32 nTeamSlot)
{
	if( nTeamSlot < 0 || nTeamSlot >= MAX_TEAM_AVATAR_COUNT )
		return false;

	return true;
}

FINISH_FACTORY_ARG0(Team);
Team::Team(void)
{
}


Team::~Team(void)
{
}

void Team::SetTeamID(int64 nID)
{
	if(m_pParamPool)
		return;

	PARAM_SET_VALUE( m_pParamPool, teamid, nID, true);
}

int64 Team::GetTeamID()
{
	if(!m_pParamPool)
		return INVALID_TEAM_ID;

	int64 nTeamID = PARAM_GET_VALUE( m_pParamPool, teamid, INVALID_TEAM_ID);
	return nTeamID;
}

int64 Team::GetLeaderDID() const
{
	if(!m_pParamPool)
		return 0;

	int64 nLeaderDID = PARAM_GET_VALUE( m_pParamPool, leaderdid, -1);
	return nLeaderDID;
}

void Team::SetLeaderDID(int64 nAvatarDID)
{
	TeamMember* pMember = GetItem( nAvatarDID );
	if(pMember == NULL)
		return;

	ParamPool* pPool = pMember->GetParamPool();
	if( pPool == NULL )
		return;

	PARAM_SET_VALUE( pPool, teampost, int32(eTeam_Post_Leader), true);
	PARAM_SET_VALUE( m_pParamPool, leaderdid, nAvatarDID, true);
}

int32 Team::GetMemberCount() const
{
	return (int32)size();
}

TeamSlotContainer::TeamSlotContainer()
{
	memset( m_slots, 0, sizeof(m_slots));
}

bool TeamSlotContainer::SlotValid(int32 nSlotID) const
{
	return TeamUtil::SlotValid(nSlotID);
}


