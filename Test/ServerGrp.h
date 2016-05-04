#pragma once

#include "MySockAddr.h"

class ServerInfo;

class ServerGrp
{
public:
	ServerGrp(void);
	virtual ~ServerGrp(void);

	virtual ServerInfo* CreateSrvInfo( int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr);
};

