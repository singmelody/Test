#pragma once

#include "SceneManagerBase.h"

class SceneManager : public SceneManagerBase
{
public:
	SceneManager(void);
	virtual ~SceneManager(void);


protected:
	int32 m_copyCnt[];
};

