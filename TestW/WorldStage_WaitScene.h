#pragma once

#include "WorldStage.h"
#include "Singleton.h"
`
class WorldStage_WaitScene : public WorldStage, public Singleton<WorldStage_WaitScene>
{
public:
	WorldStage_WaitScene(void);
	~WorldStage_WaitScene(void);

	virtual int32 GetState() { return eWS_WaitScene; }
	virtual void Tick( int32 nFrameTime );
};

