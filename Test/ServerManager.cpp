#include "StdAfx.h"
#include "ServerManager.h"


ServerManager::ServerManager(void)
{
}


ServerManager::~ServerManager(void)
{
}

int32 ServerManager::GetZoneID(int32 nSrvID)
{
	return ( nSrvID >> eSrvID_ZoneStartBit ) & eSrvID_MaxZoneID;
}

int32 ServerManager::GetGroupID(int32 nSrvID)
{
	return ( nSrvID >> eSrvID_GrpStartBit) & eSrvID_MaxGrpID;
}

int32 ServerManager::GetSrvType(int32 nSrvID)
{
	return ( nSrvID >> eSrvID_TypeBits) & eSrvID_MaxTypeID;
}

int32 ServerManager::GetSrvIndex(int32 nSrvID)
{
	return nSrvID & eSrvID_MaxIndexBits;
}

void ServerManager::SetPeerModule(PeerModuleBase* pPeerModule)
{
	m_pPeerModule = pPeerModule;
}

// zoneid	groupid		type	index
//	31-24	23 - 18	   17-13	
//
int32 ServerManager::MakeSrvID(int32 nZoneID, int32 nGrpID, int32 nSrvType, int32 nIdx)
{
	int32 nSrvID = ( nZoneID << eSrvID_ZoneStartBit) + ( nGrpID << eSrvID_GrpStartBit) 
		+ ( nSrvType << eSrvID_TypeStartBit) + nIdx;
}

int32 ServerManager::MakeSrvID(int32 nSrvType, int32 nSrvIdx)
{
	return MakeSrvID( m_nZoneID, m_nGrpID, nSrvType, nSrvIdx);
}
