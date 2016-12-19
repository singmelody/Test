#pragma once

#include "Factory.h"
#include "LogicTreeBase.h"
#include "LogicTypeDefine.h"

// logic module input & output binding info desc
struct sLogicInputInfo
{
	int16 nLogicIndex;		// output module's idx in logic group
	int16 nOutputIndex;		// output channel id
};

class MyLogic : public LogicTreeBase
{
	DECLARE_FACTORY_ARG0_Ex( LogicTreeBase, -1, new PoolAllocator(), LogicFactory::Instance());
public:
	MyLogic(void);
	virtual ~MyLogic(void);
};

