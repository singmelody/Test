#pragma once

#include "SceneManagerBase.h"
#include "SceneInfo.h"

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

class SceneContentGroup;

typedef std::map< int32, SceneContentGroup*> BornDataMap;

class Scene;

class SceneManager : public SceneManagerBase
{
public:
	SceneManager(void);
	virtual ~SceneManager(void);

	SceneInstanceMgr* GetSceneMap(uint16 nSceneSID);
	virtual Scene* CreateScene( SceneCreateArg& arg);

	virtual void OnSceneCreated( Scene* pScene);
	virtual void OnSceneDestroied( Scene* pScene);

	BornDataMap	m_bornDataMap;
protected:

	int32 m_nCopyCnt;
	int32 m_nSceneLoadValue;
};

