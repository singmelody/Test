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

	bool Init(int32 argc, char* argv[]);
	bool Start();
	virtual void Exit();
	SHMManager_DBA& GetSHMMgr() { return m_shmMgr; }

	void Send2World(PacketBase& pkt);
	void Send2LocalWorld(PacketBase& pkt);

	void OnWorldDisconnect(ServerInfo* pInfo);

	IParamPool2SqlProcessor* GetParam2SqlProcessor(int32 nParamIdx);
protected:
	class DBInterface* m_pInterface;
	SHMManager_DBA m_shmMgr;

	uint32 DBAConfig_delta_time_of_db_statics;
};

