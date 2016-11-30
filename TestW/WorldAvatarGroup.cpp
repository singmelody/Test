#include "StdAfx.h"
#include "WorldAvatarGroup.h"
#include "WorldTeamManager.h"

WorldAvatarGroup::WorldAvatarGroup(void)
{
	m_nTeamID = INVALID_TEAM_ID;
}


WorldAvatarGroup::~WorldAvatarGroup(void)
{

}

void WorldAvatarGroup::SetNodeSrvID(int32 nNewID)
{
	int32 nOldID = GetNodeSrvID();

	if( nOldID == nNewID )
		return;

	AvatarSrvEx::SetNodeSrvID(nNewID);

	WorldTeam* pTeam = GetWorldTeam();
	if(!pTeam)
		return;

	pTeam->m_NodeTrunk.RemovePlayer( this, nOldID);
	pTeam->m_NodeTrunk.AddPlayer( this, nNewID);
}

int64 WorldAvatarGroup::GetTeamID()
{
	return m_nTeamID;
}

void WorldAvatarGroup::SetTeamID(int64 nID)
{
	AvatarSrvEx::SetTeamID( nID );
	m_nTeamID = nID;
}

void WorldAvatarGroup::OnAfterPullDataFromNode()
{
	PARAM_SET_VALUE( m_pParamPool, teamid, m_nTeamID, TRUE);
}

void WorldAvatarGroup::OnAfterPullDataFromDBA()
{
	m_nTeamID = PARAM_GET_VALUE( m_pParamPool, teamid, INVALID_TEAM_ID);
}

WorldTeam* WorldAvatarGroup::GetWorldTeam()
{
	if( m_nTeamID == INVALID_TEAM_ID )
		return NULL;

	return WorldTeamManager::Instance().GetWorldTeam( m_nTeamID );
}

void WorldAvatarGroup::SetWorldTeam(WorldTeam* pTeam)
{
	if(pTeam == NULL)
		SetTeamID( INVALID_TEAM_ID);
	else
		SetTeamID( pTeam->GetTeamID() );
}

bool WorldAvatarGroup::CheckState(int32 nState)
{
	return AvatarSrvEx::CheckState(nState);
}

