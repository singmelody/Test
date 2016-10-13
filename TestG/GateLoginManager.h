#pragma once
#include "GateFuncExManager.h"
#include "Singleton.h"

class GateLoginManager : public GateFuncExManager, public Singleton<GateLoginManager>
{
public:
	GateLoginManager(void);
	~GateLoginManager(void);

	// World 2 Gate
	void RegPeerPktHandle(class PacketProcessor* pProc);

	// Clt 2 Gate
	void RegCltPktHandle(class PacketProcessor* pProc);

	void PktClt_ConnectGateSrv(class PacketConnectGateSrv* pPkt);
	void PktClt_CltReady(class PacketCltReady *pPkt);

	void PktClt_CreateAvatar(class PacketCreateAvatar *pPkt);
	void PktClt_DelCharacter(class PacketDelCharacter *pPkt);
	void PktClt_CltSelectAvatar(class PacketCltSelectAvatar *pPkt);
	void PktClt_Logout2Login(class PacketLogout2Login *pPkt);


};

