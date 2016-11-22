#pragma once

#include <string>
#include "ModuleBase.h"
#include "PeerBase.h"
#include "CircularList.h"
#include "MyMutex.h"

class ServerInfo;
class ParamPool;
class ServerManager;
class PacketAddSrvInfo;

class ConnectionItem
{
public:
	ConnectionItem();

	int32 nSrvID;
	int32 nSrvType;
	int32 nSocketID;

	int32 nListenPort;
	char  listenIP[IPLEN];
};

class SrvItem
{
public:
	SrvItem();
	SrvItem( class PacketSrvConnect* pPkt);

	int32	nSrvType;
	int32	nSrvID;
	int32	nSocketID;

	int32	listenPortPeer;
	char	listenIpPeer[IPLEN];

	int32	listenPortClt;
	char	listenIpClt[IPLEN];

	uint64	nLastConnectTime;
};

typedef std::list<ConnectionItem*>		ConnectionItemList;
typedef std::map<int32, SrvItem*>		ServerItemMap;

class PeerModuleBase : public ModuleBase, public PeerBase
{
public:
	PeerModuleBase(SrvType nModuleType);
	~PeerModuleBase(void);

	virtual bool Init(int32 argc, char* argv[]);
	virtual void ProcessLogic(int32 nFrameTime);
	virtual void Exit();
	virtual void OnConfigLoaded();
	virtual void RegPeerPktHandle(PacketProcessor* pProc);

	virtual bool ProcessPacket();
	virtual void OnServerIDChange(int32 nID){}

	virtual void UpdateDogPool(int32 nFrameTime){}
	virtual void InitDogParamPool( ServerInfo* pInfo);
	virtual void Broadcast2Dogs( ParamPool* pPool);

	bool LoginWorldServer(int32 nMaxWaitTime = -1);
	bool LoginPeerServer(std::string& ip, int32 nPort, bool bWait);

	virtual void OnAllocateServerID( int32 nSrvType, int32 nSrvID, int32 nSocketID){}
	virtual void OnAddServerInfo( ServerInfo* pInfo );

	virtual void OnRemoveServerInfo(ServerInfo* pInfo );

	virtual void Send2Login( PacketBase* pPkt);

	virtual void Send2Gate( PacketBase* pPkt, int32 nSrvID = -1, bool bGateProc = false);
	virtual void Send2Gate( PacketBase* pPkt, int32 nSrvID, uint16 nChannelID, int16 channelList[]);

	virtual void OnAddWorldInfo( ServerInfo* pInfo) {}
	virtual void OnAddLoginInfo( ServerInfo* pInfo) {}
	virtual void OnAddDBAInfo( ServerInfo* pInfo) {}
	virtual void OnAddGateInfo( ServerInfo* pInfo) {}
	virtual void OnAddNodeInfo( ServerInfo* pInfo) {}

	virtual void OnRemoveWorldInfo( ServerInfo* pInfo) {}
	virtual void OnRemoveLoginInfo( ServerInfo* pInfo) {}
	virtual void OnRemoveDBAInfo( ServerInfo* pInfo) {}
	virtual void OnRemoveGateInfo( ServerInfo* pInfo) {}
	virtual void OnRemoveNodeInfo( ServerInfo* pInfo) {}

	void StartConnectionThread();
	void ConnectThreadProc();
	void SendPacketSrvConnect(int32 nSocket);
	void ProcessConnection(int32 nFrameTime);

	void OnPeerConnected(SrvItem* pItem);
	void OnPeerDisConnect(int32 nSocketID);

	bool GetConnectingItem(class ConnectionItem& item);

	void W2GPacketCounter(int32 nPacketType);
	void N2GPacketCounter(int32 nPacketType);

	void AddConnectionItem( int32 nSrvID, int32 nSocketID);
	ConnectionItem* GetConnectedItem();
	void BroadcastServerInfo2Gate(ServerInfo* pInfo);

	SrvItem* GetSrvItem(int32 nSrvID);
	void RemoveSrvItem(int32 nSrvID);

	ServerItemMap m_mapSrvItems;
protected:
	virtual void CalculateStatistics();
	virtual void ResetStatistics();

	void SyncConnectServer(SrvItem* pInfo);

	virtual void OnWorldDisconnect(ServerInfo* pInfo) {}
	virtual void OnLocalWorldDisconnect(ServerInfo* pInfo) {}
	virtual void OnDBADisconnect(ServerInfo* pInfo) {}
	virtual void OnLoginDisconnect(ServerInfo* pInfo) {}
	virtual void OnGateDisconnect(ServerInfo* pInfo) {}
	virtual void OnNodeDisconnect(ServerInfo* pInfo) {}
	virtual void OnCollisionDisconnect(ServerInfo* pInfo) {}

	void OnRecvSrvInfoPkt(PacketAddSrvInfo* pPkt);
	void OnSendPacketSrvConnect(class PacketSrvConnect& pkt);
	void OnRecvSrvConnectPkt(class PacketSrvConnect* pPkt);
	
	virtual void PeerDisconnect(int32 nSocketID);

	void PktHandlerSrvID(class PacketSrvID* pPkt);
	void PktHandlerSrvHeartBeat(class PacketHeartBeat* pPkt);

	#define THREADMAXCNT 4
	CircularList<class MyThread*,THREADMAXCNT>	m_PeerThreadList;
	bool	m_bUseConnectionThread;

	Mutex	m_lockConnection;
	ConnectionItemList	m_listConnectedItems;
};

