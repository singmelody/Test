#pragma once

#include "WorldState.h"

class WorldState_WaitLogining : public WorldState, public Singleton<WorldState_WaitLogining>
{
public:
	WorldState_WaitLogining(void);
	~WorldState_WaitLogining(void);

	virtual int32 GetState() { return eWS_WaitingLogining; }
	virtual void Tick(int32 nFrameTime);
};

