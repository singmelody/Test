#pragma once

#include "GateFuncExManager.h"
#include "Singleton.h"

class GateChatManager : public GateFuncExManager, public Singleton<GateChatManager>
{
public:
	GateChatManager(void);
	~GateChatManager(void);

	void Init();
};

