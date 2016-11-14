#pragma once

#include "CommonDataObj.h"
#include "Factory.h"

enum ScenePermanentDefine
{
	eScenePermanent_Conquer	= 1 << 10,	// finish flag
	eScenePermanent_Reset	= 1 << 11,	// reset flag

};

class SceneProcess : public CommonDataObject
{
	DECLARE_FACTORY_ARG0(SceneProcess, -1, new PoolAllocator);
public:
	SceneProcess(void);
	virtual ~SceneProcess(void);

	void SetScenePermanent( ScenePermanentDefine bit );
	bool CheckScenePermanent(ScenePermanentDefine bit);
	void ClearScenePermanent(ScenePermanentDefine bit);

	bool HasResetFlag() { return CheckScenePermanent(eScenePermanent_Reset); }

	int64 GetProcessBits();
};

