#pragma once

#include "DBABase.h"
#include "Singleton.h"
#include "SHMManager_DBA.h"

class DBAServer : public DBABase, public Singleton<DBAServer>
{
public:
	DBAServer(void);
	virtual ~DBAServer(void);

	bool Init(int32 argc, int32 argv[]);
	bool Start();
	virtual void Exit();
	SHMManager_DBA& GetSHMMgr() { return m_shmMgr; }

protected:
	SHMManager_DBA m_shmMgr;
};

