#pragma once
#include "FactoryManagerBase.h"

#define DECLARE_FACTORY_ARG0_EX( classObj, classID) \
	private:	\
	class CClassFactory_##classObj : public FactoryBase_Arg0 \
	{	\
	public: \
		CClassFactory_##classObj() : CFactoryBase_Arg0()
	};	\
