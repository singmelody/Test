#pragma once
#include "GateFuncExManager.h"
#include "Singleton.h"

class PasskeyInfo
{
	DECLARE_FACTORY_ARG0( PasskeyInfo, -1, new PoolAllocator);
public:
	PasskeyInfo();

	std::string accountName;
	std::string key;

	int32		nSocketID;
	int32		nExpireTime;
	uint32		nAccountOnlineTime;
};

typedef std::map< UtilID, PasskeyInfo*> PasskeyInfoMap;

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

	void Tick(int32 nFrameTime);
	void ProcPassKeyInfo(int32 nFrameTime);
	void OnLoseClt();
protected:
	PasskeyInfoMap m_PasskeyInfos;
};

