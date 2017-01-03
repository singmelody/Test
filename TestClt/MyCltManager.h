#pragma once
#include "Singleton.h"

class MyCltManager : public Singleton<MyCltManager>
{
public:
	MyCltManager(void);
	~MyCltManager(void);

	void Connect2LoginSrv(const char* sAddr, int32 nPort);
	void InitNetManager();
	virtual void Init();
	virtual void Tick(bool bSleep);
};

