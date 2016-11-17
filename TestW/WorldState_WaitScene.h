#pragma once

#include "WorldState.h"
#include "Singleton.h"

class WorldState_WaitScene : public WorldState, public Singleton<WorldState_WaitScene>
{
public:
	WorldState_WaitScene(void);
	virtual ~WorldState_WaitScene(void);

	virtual int32 GetState() { return eWS_WaitScene; }
	virtual void Tick( int32 nFrameTime );
};

