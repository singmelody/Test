#pragma once

#include "Singleton.h"
#include "WorldFuncExManager.h"

enum WorldStateID
{
	eWS_Null = -1,
	eWS_Logining,		// login state
	eWS_Logined,		// login finish
	eWS_DataLoading,	// enter state, wait for the dba fetch finish
	eWS_DataReady,
	eWS_WaitScene,
	eWS_Decommission,
	eWS_EnterGame,
	eWS_Jumping,
	eWS_Gaming,
	eWS_ExitGame,
	eWS_Billing,
	eWS_WaitingLogining,
	eWS_WaitingNodeData,
	eWS_WritingDBA,
	eWS_Count
};

class WorldState;

class WorldStateManager : public WorldFuncExManager, public Singleton<WorldStateManager>
{
public:
	WorldStateManager(void);
	~WorldStateManager(void);

	WorldState* GetState( WorldStateID nStateID);

protected:
	void RegPeerPktHandle( PacketProcessor* pProc);
	WorldState*		m_worldState[eWS_Count];
};

