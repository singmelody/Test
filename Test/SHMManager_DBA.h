#pragma once

#include "SHMManagerBase.h"
#include "ParamPool2Sql_DBA.h"

class DBInterface;

class SHMParamPool2SqlProcessor_DBA_Avatar : public ParamPool2SqlProcessor_DBA_Avatar
{
	virtual bool UseSHM() const { return true; }
};

class SHMParamPool2SqlProcessor_DBA_Avatar_CommonData : public ParamPool2Sqlprocessor_DBA_Avatar_CommonData
{
	virtual bool UseSHM() const { return true; }
};

class SHMManager_DBA : public SHMManagerBase
{
public:
	SHMManager_DBA(void);
	virtual ~SHMManager_DBA(void);

	void Sync2DB();

protected:
	virtual void InitParamPool2SqlProcessors(bool bUseSHM);
};

