#include "StdAfx.h"
#include "ServerGrp.h"


ServerGrp::ServerGrp(void)
{
}


ServerGrp::~ServerGrp(void)
{
}

ServerInfo* ServerGrp::CreateSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr)
{
	return new ServerInfo( nSrvType, nSrvID, nSocketID, laddr);
}
