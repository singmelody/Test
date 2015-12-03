#pragma once

#include "ParamPool.h"

#define DECLARE_PARAMSET(x) \
	class ParamSet##x : public ParamPoolEx<x> \
	{ \
		DECLARE_FACTORY_ARG0( ParamSet##x, -1, new PoolAllocator); \
	};\

DECLARE_PARAMSET(10)
DECLARE_PARAMSET(20)