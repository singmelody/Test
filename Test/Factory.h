#pragma once
#include "FactoryManagerBase.h"

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
