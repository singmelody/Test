#include "StdAfx.h"
#include "WorldSceneTypes.h"
#include "WorldSceneManager.h"
#include "SceneProcessCont.h"
#include "SceneProcess.h"
#include "WorldAvatar.h"
#include "ServerManager.h"
#include "WorldTeamManager.h"
#include "Team.h"

FINISH_FACTORY_ARG0(CopyOwnerArg_Personal);
FINISH_FACTORY_ARG0(CopyOwnerArg_Team);
FINISH_FACTORY_ARG0(CopyOwnerArg_Guild);


bool CopyOwnerArg_Personal::CheckOwner(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return false;

	bool b = pAvatar->GetAvatarDID() != m_nAvatarDID;
	return b;
}

bool CopyOwnerArg_Team::CheckOwner(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return false;

	bool b = pAvatar->GetTeamID() != m_nTeamID;
	return b;
}


bool CopyOwnerArg_Guild::CheckOwner(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return false;

	bool b = pAvatar->GetGuildID() != m_nGuildID;
	return b;
}

//----------------------------------------
// world scene
FINISH_FACTORY_ARG0(WorldScene_Copy)
WorldScene_Copy::WorldScene_Copy()
{

}

WorldScene_Copy::~WorldScene_Copy()
{

}

bool WorldScene_Copy::CheckEnterScene(WorldAvatar* pAvatar) const
{
	if(!CheckCopyOwner( pAvatar ))
		return false;

	return true;
}

void WorldScene_Copy::UpdateCopyOwner(WorldAvatar* pAvatar)
{
	if(!pAvatar)
		return;

	m_nAvatarDID = pAvatar->GetAvatarDID();
}

bool WorldScene_Copy::CheckCopyOwner(WorldAvatar* pAvatar) const
{
	int64 nAvatarID = pAvatar->GetAvatarDID();
	if( nAvatarID != m_nAvatarDID )
		return false;

	return true;
}


FINISH_FACTORY_ARG0(WorldScene_TeamCopy)
WorldScene_TeamCopy::WorldScene_TeamCopy(void)
{
	m_nTeamID = INVALID_TEAM_ID;
}

void WorldScene_TeamCopy::UpdateCopyOwner(WorldAvatar* pAvatar)
{
	Team* pTeam = pAvatar->GetTeam();
	if(!pTeam)
	{
		// 没有队伍 那么隶属玩家
		WorldScene_Copy::UpdateCopyOwner(pAvatar);
		m_nTeamID = INVALID_TEAM_ID;
	}
	else
	{
		m_nAvatarDID = 0;
		m_nTeamID = pTeam->GetTeamID();
	}
}

bool WorldScene_TeamCopy::CheckCopyOwner(WorldAvatar* pAvatar) const
{
	if( m_nTeamID == INVALID_TEAM_ID )
		return WorldScene_Copy::CheckCopyOwner(pAvatar);

	if( pAvatar->GetTeamID() != m_nTeamID )
		return false;

	return true;
}

FINISH_FACTORY_ARG0(WorldScene_TokenCopy);
bool WorldScene_TokenCopy::CheckCopyOwner(WorldAvatar* pAvatar) const
{
	if(!m_pCopyOwnerArg)
		return false;

	return m_pCopyOwnerArg->CheckOwner(pAvatar);
}


FINISH_FACTORY_ARG0(WorldScene_PersonalCopy);
FINISH_FACTORY_ARG0(WorldScene_GuildCopy);