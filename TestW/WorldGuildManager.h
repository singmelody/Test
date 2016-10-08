#pragma once
#include "Singleton.h"
#include "WorldFuncExManager.h"

class WorldGuildManager : public WorldFuncExManager, public Singleton<WorldGuildManager>
{
public:
	WorldGuildManager(void);
	virtual ~WorldGuildManager(void);

	void OnConnectWarWorld() { SetWarConnectState(eWarServer_Connect); }
	void OnConnectWarInit() { SetWarConnectState(eWarServer_Init); }

	void OnDisconnectWarWorld();

	void SetWarConnectState(int32 nState) { m_nWarConnectState = nState;}

protected:
	int32 m_nWarConnectState;

	enum
	{
		eWarServer_Disconnect = 0,
		eWarServer_Connect,
		eWarServer_Init,
	};
};

