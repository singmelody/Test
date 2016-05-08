#pragma once

#include "AvatarSrv.h"
#include "CommonDataOwner.h"

class AvatarSrvEx : public AvatarSrv, public CommonDataOwner
{
public:
	AvatarSrvEx(void);
	virtual ~AvatarSrvEx(void);
};

