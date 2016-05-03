#pragma once

#include <string>
#include "ModuleBase.h"
#include "PeerBase.h"
#include "CircularList.h"

class ServerInfo;
class ParamPool;
class ServerManager;

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

class PeerModuleBase : public ModuleBase, public PeerBase
{
public:
	PeerModuleBase(int32 nModuleType);
	~PeerModuleBase(void);

	virtual bool Init(int32 argc, char* argv[]);
	virtual void ProcessLogic(int32 nFrameTime);
	virtual void Exit();
	virtual void OnConfigLoaded();

	virtual bool ProcessPacket();
	virtual void OnServerIDChange(int32 nID){}

	virtual void UpdateDogPool();
	virtual void InitDogParamPool( ServerInfo* pInfo);
	void Broadcast2Dogs( ParamPool* pPool);

	bool LoginWorldServer(int32 nMaxWaitTime = -1);
	bool LoginPeerServer(std::string& ip, int32 nPort, bool bWait);

	virtual void OnAllocateServerID( int32 nSrvType, int32 nSrvID, int32 nSocketID){}
	virtual void OnAddServerInfo( ServerInfo* pInfo );

	virtual void OnRemoveServerInfo(ServerInfo* pInfo );

	virtual void Send2Login( PacketBase* pPkt);

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

	bool GetConnectionItem(class ConnectionItem& item);
protected:
	ServerManager& Servers;

	#define THREADMAXCNT 4
	CircularList<class MyThread*,THREADMAXCNT>	m_PeerThreadList;
	bool m_bUseConnectionThread;
};

