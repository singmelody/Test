#pragma once

#include "SHMManagerBase.h"

class DBInterface;

class SHMManager_DBA : public SHMManagerBase
{
public:
	SHMManager_DBA(void);
	virtual ~SHMManager_DBA(void);

	void Sync2DB();

protected:
	virtual void InitParamPool2SqlProcessors(bool bUseSHM);
};

