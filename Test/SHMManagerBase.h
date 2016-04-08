#pragma once

#include "ParamDefManager.h"
#include "ParamPool2SqlProcessor.h"


class SHMManagerBase : public ParamDefManager::IListener, public ParamPool2SqlProcessorManagerBase
{
public:
	SHMManagerBase(void);
	virtual ~SHMManagerBase(void);
};

