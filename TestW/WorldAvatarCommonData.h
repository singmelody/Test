#pragma once

#include "Factory.h"
#include "CommonDataManager.h"

class WorldAvatarCommonData : public CommonDataManagerGame
{
	DECLARE_FACTORY_ARG0(WorldAvatarCommonData, -1, new PoolAllocator)
public:
	WorldAvatarCommonData(void);
	virtual ~WorldAvatarCommonData(void);
};

