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