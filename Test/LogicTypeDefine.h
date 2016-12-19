#pragma once

#include "Singleton.h"
#include "FactoryManagerBase.h"

#define REG_OUTPUT_CHAR( classObj, member, size, des) \
	LogicOutput_Char* pOutput_##member = new LogicOutputChar(static_cast<int32>);


class LogicFactory : public FactoryManagerBase, public Singleton<LogicFactory>
{
public:
	LogicFactory(){}
	virtual ~LogicFactory(){}
};