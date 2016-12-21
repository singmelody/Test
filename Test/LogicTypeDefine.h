#pragma once

#include "Singleton.h"
#include "FactoryManagerBase.h"

#define LOGIC_MAX_INPUT				16
#define LOGIC_MAX_OUTPUT			16
#define MAX_LOGIC_SIZE_IN_GROUP		128
#define MAX_LOGIC_GROUP_SIZE		8192
#define LOGIC_CHAR_SIZE				32

enum
{
	eLogic_InputOutput_Base = -1,
	eLogic_InputOutput_Float,
	eLogic_InputOutput_Int,
	eLogic_InoutOutput_Char,
	eLogic_InoutOutput_Ptr,
	eLogic_InoutOutput_Vector,
};

#define Logic_Ref_Base(className) \
	Ref_BaseClass_BeginEx( className, LogicClassManager::Instance())

#define REG_OUTPUT_CHAR( classObj, member, size, des) \
	LogicOutput_Char* pOutput_##member = new LogicOutputChar(static_cast<int32>);


class LogicFactory : public FactoryManagerBase, public Singleton<LogicFactory>
{
public:
	LogicFactory(){}
	virtual ~LogicFactory(){}
};