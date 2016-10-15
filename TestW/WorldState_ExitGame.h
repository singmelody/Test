#pragma once

#include "WorldStateGame.h"
#include "Singleton.h"

class WorldState_ExitGame : public WorldStateGameNode, public Singleton<WorldState_ExitGame>
{
public:
	WorldState_ExitGame(void);
	~WorldState_ExitGame(void);

	virtual int32 GetState() { return eWS_ExitGame; }

	virtual void OnEnterState(WorldAvatar* pAvatar);

	virtual void Tick(int32 nFrameTime);
	virtual void OnPullAvatarDataFinish(WorldAvatar* pAvatar);
	virtual void DestroyAvatar(WorldAvatar* pAvatar);
	virtual void OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData);
};

