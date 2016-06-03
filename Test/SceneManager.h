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

enum ChangeSceneRst
{
	eChangeSceneSucceed = 0,		
	eChangeSceneError_SceneIsFull,		// player max
	eChangeSceneError_SceneIsClosing,
	eChangeSceneError_SceneIsBlocking,	// scene is freezon block
	eChangeSceneError_CheckEnterFailed,	
	
	eChangeSceneError_WarContext,
	eChangeSceneError_SceneNotExist,

	eChangeSceneError_CreateNodeAvatar,
	eChangeSceneError_AddNodeAvatar,
	eChangeSceneError_NodeID,
};

class SceneManager : public SceneManagerBase
{
public:
	SceneManager(void);
	virtual ~SceneManager(void);


protected:
	int32 m_copyCnt[];
};

