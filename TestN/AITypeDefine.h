#pragma once

#include "Singleton.h"
#include "FactoryManagerBase.h"

#define AIUNIT_FACTORY(aiunit) \
	DECLARE_FACTORY_ARG0_Ex( aiunit, -1, new PoolAllocator, AIUnitFactory::Instance())

#define AIUNIT_FACTORY_FINISH(aiunit) \
	FINISH_FACTORY_ARG0(aiunit)

class AIUnitFactory : public FactoryManagerBase, public Singleton<AIUnitFactory>
{
public:
	AIUnitFactory(){}
	virtual ~AIUnitFactory(){}
};

