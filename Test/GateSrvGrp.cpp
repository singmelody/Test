#include "StdAfx.h"
#include "GateSrvGrp.h"
#include <algorithm>

GateSrvInfo::GateSrvInfo()
	: ServerInfo()
{
	m_listenPortClt = -1;
	memset( listenIPClt, 0, IPLEN);
	m_cltCnt = 0;
}

GateSrvInfo::GateSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& addr)
	: ServerInfo( nSrvType, nSrvID, nSocketID, addr)
{
	m_listenPortClt = -1;
	memset( listenIPClt, 0, IPLEN);
	m_cltCnt = 0;
}

GateSrvInfo::~GateSrvInfo(void)
{

}

void GateSrvInfo::UpdateDetailsPool()
{
	ParamPool*& pPool = m_pParamDetails;
	if( !pPool )
	{
		int32 nIdx = ServerManager::GetSrvIndex( nSrvID );
		pPool = ParamSet::CreateNew( eParam_LoginGateDetails, nIdx);
	}

	PARAM_SET_VALUE( pPool, ServerID, nSrvID, true);
	PARAM_SET_VALUE( pPool, CltCnt, m_cltCnt, true);
}

void GateSrvInfo::FillPacket(PacketAddSrvInfo* pPkt)
{
	ServerInfo::FillPacket(pPkt);

	pPkt->nListenPortClt = m_listenPortClt;
	memcpy( pPkt->ListenIpClt, listenIPClt, sizeof(listenIPClt));
}

void GateSrvInfo::SetGateSrvInfo( int32 nPort, char* pIP)
{
	ParamPool*& pPool = m_pParamDetails;
	if(!pPool)
	{
		int32 nIdx = ServerManager::GetSrvIndex( nSrvID );
		pPool = ParamSet::CreateNew( eParam_WorldNodeDetails, nIdx);
	}

	PARAM_SET_VALUE( pPool, ServerID, nSrvID, true);
	PARAM_SET_VALUE( pPool, CltCnt, m_cltCnt, true);
}

ServerInfo* GateSrvGrp::CreateSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	return new GateSrvInfo( nSrvType, nSrvID, nSocketID, laddr);
}
