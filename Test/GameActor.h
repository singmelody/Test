#pragma once

#include "ParamPoolOwner.h"
#include "Avatar.h"

class GameActor : public virtual ParamPoolOwner, public virtual Avatar
{
public:
	GameActor(void);
	virtual ~GameActor(void);
};

