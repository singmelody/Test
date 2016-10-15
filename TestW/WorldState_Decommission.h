#pragma once
#include "Singleton.h"
#include "WorldStateGame.h"

// pull data from node ( when node is leaving etc..)
class WorldState_Decommission : public WorldStateGameNode, public Singleton<WorldState_Decommission>
{
public:
	WorldState_Decommission(void);
	~WorldState_Decommission(void);

	virtual int32 GetState() { return eWS_Decommission; }
	virtual void Tick( int32 nFrameTime );

	virtual void OnPullAvatarDataFinish(WorldAvatar* pAvatar);
	virtual void OnPullAvatarDataFailed(WorldAvatar* pAvatar);

	virtual void DestroyAvatar( WorldAvatar* pAvatar);
	virtual void OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool WaitData);

	virtual void OnCreateNodeAvatarSucceed(WorldAvatar* pAvatar);
	virtual void OnCreateNodeAvatarFailed(WorldAvatar* pAvatar, int32 nErrorID);
};

