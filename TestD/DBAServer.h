#pragma once

#include "DBABase.h"
#include "Singleton.h"
#include "SHMManager_DBA.h"

class PacketBase;

class DBAServer : public DBABase, public Singleton<DBAServer>
{
public:
	DBAServer(void);
	virtual ~DBAServer(void);

	bool Init(int32 argc, int32 argv[]);
	bool Start();
	virtual void Exit();
	SHMManager_DBA& GetSHMMgr() { return m_shmMgr; }

	void Send2World(PacketBase& pkt);
	void Send2LocalWorld(PacketBase& pkt);

	void OnWorldDisconnect(ServerInfo* pInfo);
protected:
	SHMManager_DBA m_shmMgr;
};

