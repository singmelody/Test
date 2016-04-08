#pragma once

#include "ParamPool.h"

class DBABase
{
public:
	DBABase(void);
	~DBABase(void);

	ParamPool* m_pDBADogPool;

	virtual bool CreateDogPool();
	virtual void UpdateDogPool(int32 nFrameTime);
};

