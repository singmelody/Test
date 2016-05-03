#pragma once

#include "Singleton.h"

class WorldAvatarManager : public Singleton<WorldAvatarManager>
{
public:
	WorldAvatarManager(void);
	~WorldAvatarManager(void);
};

