#pragma once

#include "Singleton.h"
#include "AsynContextManager.h"

class CreateTeamContext;

class WorldAsynContextManager : public AsynContextManager, public Singleton<WorldAsynContextManager>
{
public:
	WorldAsynContextManager(void);
	~WorldAsynContextManager(void);

	AsynContext* NewContext(char* pClassName);
	void FreeContext( AsynContext* pContext);
	CreateTeamContext* GetTeamContext( int32 nID);
};

