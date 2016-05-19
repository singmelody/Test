#pragma once

#include "SceneManagerBase.h"

enum CreateSceneRst
{
	eCreateSceneSucceed = 0,
	eCreateSceneError_Creater,	// creator not exist
	eCreateSceneError_SceneInfo,// scene template info not exist
	eCreateSceneError_Team,		// create team not exist
	eCreateSceneError_CreateTrunk,	// create other type except copy
};

class SceneManager : public SceneManagerBase
{
public:
	SceneManager(void);
	virtual ~SceneManager(void);


protected:
	int32 m_copyCnt[];
};

