#include "StdAfx.h"
#include "WorldGroup.h"
#include "WorldAvatar.h"

WorldGroup::WorldGroup(void)
{
}


WorldGroup::~WorldGroup(void)
{
}

void WorldGroup::UpdateGateTrunk(WorldAvatar* pAvatar, bool bOnline)
{
	if( bOnline )
		m_GateTrunk.AddPlayer( pAvatar, pAvatar->GetGateSrvID() );
	else
		m_GateTrunk.RemovePlayer( pAvatar, pAvatar->GetGateSrvID() );
}

void WorldGroup::UpdateNodeTrunk(WorldAvatar* pAvatar, bool bOnline)
{
	if ( bOnline )
		m_NodeTrunk.AddPlayer( pAvatar, pAvatar->GetNodeSrvID());
	else
		m_NodeTrunk.RemovePlayer( pAvatar, pAvatar->GetNodeSrvID() );
}
