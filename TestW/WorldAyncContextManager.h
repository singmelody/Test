#pragma once

#include "Singleton.h"

class WorldAyncContextManager : public Singleton<WorldAyncContextManager>
{
public:
	WorldAyncContextManager(void);
	~WorldAyncContextManager(void);
};

