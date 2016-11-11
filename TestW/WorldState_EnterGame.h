#pragma once
#include "WorldStateGame.h"
#include "Singleton.h"

class WorldState_EnterGame : public WorldStateGame, public Singleton<WorldState_EnterGame>
{
public:
	WorldState_EnterGame(void);
	~WorldState_EnterGame(void);

	virtual int32 GetState() { return eWS_EnterGame; }
	virtual void Tick(int32 nFrameTime);

	virtual void Destroy(WorldAvatar* pAvatar);
	virtual void OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData);

	virtual void OnCreateNodeAvatarSucceed(WorldAvatar* pAvatar);
	virtual void OnCreateNodeAvatarFailed(WorldAvatar* pAvatar, int32 nErrorID);
};

