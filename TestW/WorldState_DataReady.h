#pragma once
#include "WorldState.h"
#include "Singleton.h"

class WorldState_DataReady : public WorldState, public Singleton<WorldState_DataReady>
{
public:
	WorldState_DataReady(void);
	virtual ~WorldState_DataReady(void);

	virtual int32 GetState() { return eWS_DataReady; }
	virtual void OnEnterStage( WorldAvatar* pAvatar);
	virtual void OnLeaveStage( WorldAvatar* pAvatar);
	virtual void DestroyAvatar( WorldAvatar* pAvatar);

	virtual void Tick(int32 nFrameTime);

protected:
	bool RequestEnterScene( WorldAvatar* pAvatar, int32 nTargetSceneID);
	bool TryEnterNextScene( WorldAvatar* pAvatar);
};

