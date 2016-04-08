#pragma once

#include "ParamPool2Sql_DBA.h"

class SHMParamPool2SqlProcessor_DBA_Avatar : public ParamPool2SqlProcessor_DBA_Avatar
{
	virtual bool UseSHM() const { return true; }
};