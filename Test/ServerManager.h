#pragma once

#include "Singleton.h"
#include "ServerGrpInfo.h"

class PeerModuleBase;
class ServerInfo;

class ServerManager : public Singleton<ServerManager>
{
public:
	ServerManager(void);
	~ServerManager(void);

	static int32 GetZoneID(int32 nSrvID);
	static int32 GetGroupID(int32 nSrvID);
	static int32 GetSrvType(int32 nSrvID);
	static int32 GetSrvIndex(int32 nSrvID);

	static int32 MakeSrvID( int32 nZoneID, int32 nGrpID, int32 nSrvType, int32 nIdx);

	void SetPeerModule(PeerModuleBase* pPeerModule);

protected:

	int32			m_localWorldChannelID;

	ServerInfo*		m_pLocalWorld;
	ServerInfo*		m_pLogin;
	ServerInfo*		m_pDBA;
	ServerInfo*		m_pGMI;


	PeerModuleBase* m_pPeerModule;
	SrvType			m_srvType;
};

