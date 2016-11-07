#pragma once
#include "FactoryManagerBase.h"
#include "PoolAllocatorEx.h"


#define DECLARE_FACTORY_ARG0( classObj, classID, allocator) \
	private:	\
	class ClassFactory_##classObj : public Factory_Arg0<classObj> \
	{	\
	public: \
		ClassFactory_##classObj() : Factory_Arg0<classObj>(allocator) \
		{ \
			ClassID(classID); ClassName(#classObj); FactoryManager::Instance().AddFactory(this); \
		} \
		~ClassFactory_##classObj() {} \
	}; \
	static ClassFactory_##classObj m_FactoryArg0_##classObj;


#define DECLARE_FACTORY_ARG0_Ex( classObj, classID, alloc, mgr) \
	private:\
	class ClassFactory_##classObj : public Factory_Arg0<classObj> \
	{\
	public:\
		ClassFactory_##classObj() : Factory_Arg0<classObj>(alloc) {\
			ClassID(classID); ClassName(#classObj); mgr.AddFactory(this); \
		} \
		~ClassFactory_##classObj() {} \
	}; \
	static ClassFactory_##classObj m_FactoryArg0_##classObj;

#define FINISH_FACTORY_ARG0(classObj) \
	classObj::ClassFactory_##classObj classObj::m_FactoryArg0_##classObj;

#define DECLARE_FACTORY_ARG1( classObj, classID, allocator, Arg) \
	private:	\
class ClassFactory_Arg1_##classObj : public Factory_Arg1<classObj, Arg> \
	{	\
	public: \
	ClassFactory_Arg1_##classObj() : Factory_Arg1<classObj, Arg>(allocator) \
		{ \
		ClassID(classID); ClassName(#classObj); FactoryManager_Arg1<Arg>::Instance().AddFactory(this);  \
		} \
		~ClassFactory_Arg1_##classObj() {} \
	}; \
	static ClassFactory_Arg1_##classObj m_FactoryArg1_##classObj;


#define FINISH_FACTORY_ARG1(classObj) \
	classObj::ClassFactory_Arg1_##classObj classObj::m_FactoryArg1_##classObj;