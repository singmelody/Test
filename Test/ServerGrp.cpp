#include "StdAfx.h"
#include "ServerGrp.h"
#include "ServerGrpInfo.h"
#include "ServerManager.h"

ServerGrp::ServerGrp(void)
{
}


ServerGrp::~ServerGrp(void)
{
}

void ServerGrp::Init(ServerManager* pMgr, int32 nSrvType)
{
	m_pMgr = pMgr;
	m_nSrvType = nSrvType;

	m_strSrvName = GetSrvTitle( nSrvType);
}

ServerInfo* ServerGrp::CreateSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	return new ServerInfo( nSrvType, nSrvID, nSocketID, laddr);
}
