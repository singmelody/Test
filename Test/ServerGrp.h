#pragma once

#include "MySockAddr.h"
#include <map>
#include <string>

class ServerInfo;
class ServerManager;

typedef std::map< int32, ServerInfo*> SrvInfoMap;

class ServerGrp
{
public:
	ServerGrp(void);
	virtual ~ServerGrp(void);

	void Init(ServerManager* pMgr, int32 nSrvType);
	virtual ServerInfo* CreateSrvInfo( int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr);

	virtual void SortList(){}

protected:
	SrvInfoMap		m_SrvMap;

	ServerManager*	m_pMgr;
	int32			m_nSrvType;
	std::string		m_strSrvName;
};

