#pragma once

#include "PeerSrvModuleBase.h"

class LoginBase : public PeerSrvModuleBase
{
public:
	LoginBase(void);
	virtual ~LoginBase(void);

	ParamPool* m_pLoginDogPool;

	virtual bool CreateDogPool();
	virtual void UpdateDogPool(int32 nFrameTime);

	virtual void OnAddServerInfo( ServerInfo* pInfo );
	virtual void OnRemoveServerInfo(ServerInfo* pInfo );

	// override ModuleBase functions
	virtual void ProcessLogic(int32 nFrameTime);
	virtual bool ProcessPacket();
	virtual void ProcessConsole();
protected:
	void OnServerInfoChanged(ServerInfo* pInfo);

};

