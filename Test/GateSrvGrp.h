#pragma once

#include <vector>
#include "ServerGrp.h"
#include "ServerGrpInfo.h"

class Scene;

class GateSrvInfo : public ServerInfo
{
public:
	GateSrvInfo();
	GateSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& addr);
	~GateSrvInfo(void);

	virtual void UpdateDetailsPool();

	void SetGateInfo( int32 nPort, char* pIP);
	virtual void FillPacket( PacketAddSrvInfo* pPkt);

	int32	m_cltCnt;
	int32	m_listenPortClt;
	char	listenIPClt[IPLEN];

};


class GateSrvGrp : public ServerGrp
{
public:
	GateSrvGrp() {}
	~GateSrvGrp(){}

	virtual ServerInfo* CreateSrvInfo( int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr);
};