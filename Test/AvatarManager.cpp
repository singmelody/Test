#include "StdAfx.h"
#include "AvatarManager.h"
#include <assert.h>
#include "ServerManager.h"
#include "AvatarSrv.h"

AvatarManager::AvatarManager(void)
{
	m_nIDCounter = 0;
	m_nSrvGroupID = 0;
}


AvatarManager::~AvatarManager(void)
{
}


int32 AvatarManager::GenNewID()
{
	m_nIDCounter = ( m_nIDCounter + 1) % ePlayerAvatarID_MaxIdx;
	if( m_nIDCounter <= 0)
		m_nIDCounter = 1;
	return m_nIDCounter;
}

bool AvatarManager::AddPlayerAvatar(AvatarSrv* pAvatar)
{
	assert( pAvatar );
	assert( m_nSrvGroupID > 0 && m_nSrvGroupID <= ePlayerAvatarID_MaxGrp);

	bool bSucceed = false;
	int32 nOriIDCounter = m_nIDCounter;

	do
	{
		int32 nPlayerAvatarID = ( GenNewID() << ePlayerAvatarID_GrpBits) | m_nSrvGroupID;

		if(AddPlayerAvatar( nPlayerAvatarID, pAvatar))
		{
			bSucceed = true;
			break;
		}
	}while ( nOriIDCounter != m_nIDCounter );

	return bSucceed;
}

bool AvatarManager::AddPlayerAvatar(int32 nAvatarID, AvatarSrv* pAvatar)
{
	if(!insert( std::make_pair( nAvatarID, pAvatar)).second)
		return false;

	pAvatar->SetAvatarID( nAvatarID );
	return true;
}

bool AvatarManager::RemovePlayerAvatar(int32 nAvatarID)
{
	iterator itr = find(nAvatarID);
	if( itr == end() )
		return false;

	erase(itr);
	
	return true;
}

AvatarSrv* AvatarManager::GetAvatar(int32 nAvatarID)
{
	iterator itr = find( nAvatarID );
	if( itr == end() )
		return NULL;

	return itr->second;
}

void AvatarManager::SetSrvGroupID(int32 nGroupID)
{
	m_nSrvGroupID = nGroupID & ePlayerAvatarID_MaxGrp;
}

