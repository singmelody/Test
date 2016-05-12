#pragma once

#include "Singleton.h"

class WorldAsynContextManager : public Singleton<WorldAsynContextManager>
{
public:
	WorldAsynContextManager(void);
	~WorldAsynContextManager(void);
};

