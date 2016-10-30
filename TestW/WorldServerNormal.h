#pragma once
#include "WorldServer.h"
#include "MyMutex.h"

class PacketProcessor;

class WorldServerNormal : public WorldServer
{
public:
	WorldServerNormal(void);
	~WorldServerNormal(void);

	virtual bool Init(int32 nArgc, char* argv[]);
	virtual void ProcessLogic(int32 nFrameTime);
	virtual void RegPeerPktHandle(PacketProcessor* pProc);

	void PacketWorldWarInit(class PacketWorldWarInit* pPkt);

	virtual void OnConfigLoaded();
	virtual void AppendLoadTemplate();
	virtual bool ClusterCheck();

	virtual void OnWorldConnect(int32 nSrvID, int32 nSocketID, bool bWarGrp, SockAddr& laddr);
	virtual void OnWorldDisconnect(ServerInfo* pInfo);

	virtual void OnRecvSrvInfoPkt(PacketAddSrvInfo* pPkt);
	virtual void OnSrvConnectError(int32 nSrvID, int32 nErrorID);

	virtual void OnAddLoginInfo(ServerInfo* pInfo);
	virtual void OnAddNodeInfo(ServerInfo* pInfo);
	virtual void OnAddGateInfo(ServerInfo* pInfo);

	virtual void OnRemoveNodeInfo( ServerInfo* pInfo);

	virtual void OnGMIServerStateReq(class PacketGMIServerStateReq* pPkt);
	virtual void OnRecvGMIQuery(class PacketGMIQuery* pPkt);

	virtual void OnWriteWorldData2DB();

	void SendServerInfo2GMI(ServerInfo* pInfo);

	void StartLoginWarThread();
	void LoginWarThreadProc();
	void TryLoginWarServer();

	void CleanupWarNodeServers();

	Mutex			m_lockLoginWar;
	std::string		m_WarIP;
	int32			m_WarPort;

	int32			m_nGateCount;

	enum
	{
		eCWS_Waiting = 0,
		eCWS_Connect,
		eCWS_Succeed,
		eCWS_Init,
	};

	int32			m_nConnectWarState;
	int32			m_nConnectWarInterval;
};

