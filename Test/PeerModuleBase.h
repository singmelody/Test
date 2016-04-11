#pragma once

#include <string>

class ServerInfo;
class ParamPool;

class PeerModuleBase
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
};
