#pragma once

#include "PeerModuleBase.h"

class ParamPool;

class DBABase : public PeerModuleBase
{
public:
	DBABase(void);
	~DBABase(void);

	ParamPool* m_pDBADogPool;

	virtual bool CreateDogPool();
	virtual void UpdateDogPool(int32 nFrameTime);
};

