#pragma once

#include "Singleton.h"
#include "FactoryManagerBase.h"
#include "MyClassManager.h"

enum AI_Flag
{
	eAI_Flag_PreValue_Damage		= 0,
	eAI_Flag_PreValue_StrikeBack	= 1,

};

class AIUnitFactory : public FactoryManagerBase, public Singleton<AIUnitFactory>
{
public:
	AIUnitFactory(){}
	virtual ~AIUnitFactory(){}
};

class AIUnitClassManager : public MyClassManagerBase, public Singleton<AIUnitClassManager>
{
public:
	AIUnitClassManager(){}
	virtual ~AIUnitClassManager(){}
};

class AIConditionFactory : public FactoryManagerBase, public Singleton<AIConditionFactory>
{
public:
	AIConditionFactory(){}
	virtual ~AIConditionFactory(){}
};

class AICondClassManager : public MyClassManagerBase, public Singleton<AICondClassManager>
{
public:
	AICondClassManager();
	virtual ~AICondClassManager();
};

// AIUnit
#define AIUNIT_FACTORY(aiunit) \
	DECLARE_FACTORY_ARG0_Ex( aiunit, -1, new PoolAllocator, AIUnitFactory::Instance())

#define AIUNIT_FACTORY_FINISH(aiunit) \
	FINISH_FACTORY_ARG0(aiunit)

#define AIUNIT_Ref_Base(className) \
	Ref_BaseClass_BeginEx( className, AIUnitClassManager::Instance())

#define AIUNIT_Ref( className, BaseClass) \
	Ref_Class_BeginEx( className, BaseClass, AIUnitClassManager::Instance())

#define AIUNIT_Ref_End(className) \
	Ref_BaseClass_End(className)

// AICond
#define AICOND_FACTORY(className) \
	Ref_BaseClass_BeginEx( className, -1, new PoolAllocator, AIConditionFactory::Instance())

#define AICOND_FINISH(className) \
	FINISH_FACTORY_ARG0(className)

#define AICOND_Ref_Base(className) \
	Ref_BaseClass_BeginEx( className, AICondClassManager::Instance())

#define AICOND_Ref(className, BaseClass) \
	Ref_Class_BeginEx( className, BaseClass, AICondClassManager::Instance())

#define AICOND_REF_END(className) \
	Ref_BaseClass_End(className)

