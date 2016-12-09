#pragma once

#include "ModuleFuncManager.h"

class DBAFuncManager : public ModuleFuncManager
{
public:
	DBAFuncManager(void);
	~DBAFuncManager(void);

	class DBAServer& DBASrv;
	Char* GetBuffer();
};

