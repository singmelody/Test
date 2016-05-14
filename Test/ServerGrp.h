#pragma once

#include "MySockAddr.h"
#include <map>

class ServerInfo;

typedef std::map< int32, ServerInfo*> SrvInfoMap;

class ServerGrp
{
public:
	ServerGrp(void);
	virtual ~ServerGrp(void);

	virtual ServerInfo* CreateSrvInfo( int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr);

	virtual void SortList(){}

protected:
	SrvInfoMap m_SrvMap;
};

