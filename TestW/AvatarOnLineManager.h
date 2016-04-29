#pragma once

#include "Singleton.h"

class AvatarOnLineManager : public Singleton<AvatarOnLineManager>
{
public:
	AvatarOnLineManager(void);
	~AvatarOnLineManager(void);
};

