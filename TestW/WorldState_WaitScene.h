#pragma once

#include "WorldState.h"
#include "Singleton.h"
`
class WorldStateWaitScene : public WorldState, public Singleton<WorldStateWaitScene>
{
public:
	WorldStateWaitScene(void);
	~WorldStateWaitScene(void);

	virtual int32 GetState() { return eWS_WaitScene; }
	virtual void Tick( int32 nFrameTime );
};

