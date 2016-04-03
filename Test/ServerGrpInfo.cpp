#include "StdAfx.h"
#include "ServerGrpInfo.h"
#include "MySockAddr.h"
#include "FactoryManagerBase.h"
#include "PacketImpl.h"

ServerInfo::ServerInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	nSrvType = eSrv_Unkown;
	nSrvID = SrvID_NULL;
	nSocketID = -1;

	nListenPortPeer = -1;
	memset( ListenIPPeer, 0, IPLEN);
	memcpy( ListenIPPeer, laddr.GetIP().c_str(), laddr.GetIP().length());

	m_pParentGrp = NULL;
	m_DogDataTimeStamp = 0;
	m_pParamDetails = NULL;
}

ServerInfo::~ServerInfo()
{
	DogDataMap& map = m_DogDataMap;

	while ( map.size() > 0)
	{
		DogDataMap::iterator itr = map.begin();
		FACTORY_DELOBJ( itr->second );
		map.erase(itr);
	}

	if( m_pParamDetails != NULL )
		FACTORY_DELOBJ( m_pParamDetails );
}


void ServerInfo::FillPacket(PacketAddSrvInfo* pPkt)
{
	if(!pPkt)
		return;

	pPkt->nSrvType = nSrvType;
	pPkt->nSrvID = nSrvID;

	std::string sIp = ListenIPPeer;

	pPkt->nListenPortPeer = nListenPortPeer;
	memset( pPkt->ListenIpPeer, 0, IPLEN);
	memcpy( pPkt->ListenIpPeer, sIp.c_str(), sIp.length());
	memset( pPkt->ListenIpClt, 0, IPLEN);


	pPkt->SetPacketType(ePacketType_GateProc);
}