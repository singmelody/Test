#pragma once

#include "CommonDataObj.h"
#include "CommonDataCont.h"
#include "CommonDataCont_T.h"
#include "SceneProcess.h"
#include <map>

#define MAX_SCENEPROCESS_COUNT 128

typedef std::map< uint16, SceneProcess*> SceneProcessMap;

class SceneProcessCont : public CommonDataCont_T<eCommonData_SceneProcess, MAX_SCENEPROCESS_COUNT>
{
public:
	SceneProcessCont(void);
	~SceneProcessCont(void);

	SceneProcess*	GetProcess( uint16 nSceneSID);

	int64 GetProcessBits();

	SceneProcessMap	m_mapProcess;
};

