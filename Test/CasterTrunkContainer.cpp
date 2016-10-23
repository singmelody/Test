#include "StdAfx.h"
#include "CasterTrunkContainer.h"

FINISH_FACTORY_ARG0(PlayerBranchContainer)

PlayerBranchContainer::PlayerBranchContainer()
{
	m_pParentTrunk = NULL;
	m_nBranchID = -1;
}

PlayerBranchContainer::~PlayerBranchContainer()
{
	m_mapPlayers.clear();
}

bool PlayerBranchContainer::AddPlayer(int32 castID, AvatarSrv* pPlayer)
{
	m_mapPlayers[castID] = pPlayer;
	return true;
}

bool PlayerBranchContainer::RemovePlayer(int32 castID, AvatarSrv*)
{
	PlayerMap& map = m_mapPlayers;

	PlayerMap::iterator itr = map.find(castID);
	if( itr == map.end() )
		return false;

	map.erase(itr);
	return true;
}

CasterTrunkContainer::CasterTrunkContainer(bool bByNode) 
	: m_bByNode(bByNode)
{

}

CasterTrunkContainer::~CasterTrunkContainer(void)
{
	PlayerBranchMap& map = m_mapGateBranches;
	while ( map.size() > 0)
	{
		PlayerBranchMap::iterator itr = map.begin();
		RemoveBranch( itr->second );
	}
}

void CasterTrunkContainer::AddPlayer(AvatarSrv* pPlayer)
{
	if(!pPlayer)
		return;

	int32 nBranchID = m_bByNode ? pPlayer->GetNodeSrvID() : pPlayer->m_nGateSrvID;
	AddPlayer( pPlayer, nBranchID);
}

void CasterTrunkContainer::AddPlayer(AvatarSrv* pPlayer, int32 nBranchID)
{
	if( nBranchID != SERVERID_NULL )
	{
		PlayerBranchContainer* pBranch = GetBranch( nBranchID, true);
		if( pBranch )
		{
			if( m_bByNode )
			{
				int32 nAvatarID = pPlayer->GetAvatarID();
				pBranch->AddPlayer( nAvatarID, pPlayer);
			}
			else
				pBranch->AddPlayer( pPlayer->m_nGateChannelID, pPlayer);
		}
	}
}

void CasterTrunkContainer::RemovePlayer(AvatarSrv* pPlayer)
{
	if(!pPlayer)
		return;

	int32 nBranchID = m_bByNode ? pPlayer->GetNodeSrvID() : pPlayer->m_nGateSrvID;
	RemovePlayer( pPlayer, nBranchID);
}

void CasterTrunkContainer::RemovePlayer(AvatarSrv* pPlayer, int32 nBranchID)
{
	PlayerBranchContainer* pBranch = GetBranch( nBranchID, true);
	if( pBranch )
	{
		int32 nID = m_bByNode ? pPlayer->GetAvatarID() : pPlayer->m_nGateChannelID;
		if( pBranch->RemovePlayer( nID, pPlayer) )
		{
			if( pBranch->m_mapPlayers.size() == 0)
				RemoveBranch( pBranch );
		}
	}
}

PlayerBranchContainer* CasterTrunkContainer::GetBranch( int32 nBranchID, bool bCreateNew)
{
	PlayerBranchMap& map = m_mapGateBranches;
	PlayerBranchMap::iterator itr = map.find( nBranchID );

	if( itr != map.end() )
		return itr->second;

	if(!bCreateNew)
		return NULL;

	PlayerBranchContainer* pBranch = FACTORY_NEWOBJ( PlayerBranchContainer );
	if(!pBranch)
		return NULL;

	map[nBranchID] = pBranch;
	pBranch->m_pParentTrunk = this;
	pBranch->m_nBranchID = nBranchID;

	return pBranch;
}


void CasterTrunkContainer::RemoveBranch( PlayerBranchContainer* pBranch )
{
	if(!pBranch)
		return;

	PlayerBranchMap& map = m_mapGateBranches;
	PlayerBranchMap::iterator itr = map.find( pBranch->m_nBranchID);
	if( itr == map.end() )
		return;

	FACTORY_DELOBJ( itr->second );
	map.erase(itr);
}


