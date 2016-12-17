#pragma once

#include "PeerModuleBase.h"

class NodeBase : public PeerModuleBase
{
public:
	NodeBase(void);
	virtual ~NodeBase(void);

	ParamPool* m_pNodeDogPool;

	virtual bool CreateDogPool();
	virtual void UpdateDogPool(int32 nFrameTime);

	virtual void OnAddServerInfo( ServerInfo* pInfo );
	virtual void OnRemoveServerInfo(ServerInfo* pInfo );

protected:
	void OnServerInfoChanged(ServerInfo* pInfo);

	std::string m_strNodeDataSyncArgs;
};

