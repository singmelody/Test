#pragma once

#include "SceneManagerBase.h"
#include "SceneInfo.h"

enum CreateSceneRst
{
	eCreateScene_Succeed		= 0,
	eCreateSceneError_Creater	= 1,	// creator not exist
	eCreateSceneError_SceneInfo	= 2,	// scene template info not exist
	eCreateSceneError_Team		= 3,	// create team not exist
	eCreateSceneError_CreateTrunk = 4,	// create other type except copy
	eCreateSceneError_SceneNotExist = 6,
	eCreateSceneError_Timeout	= 8,
	eCreateSceneError_TeamCopy	= 9,
	eCreateSceneError_GuildNotExist		= 12,
	eCreateSceneError_CreateWorldScene	= 13,
	eCreateSceneError_CreateNodeScene	= 14,
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
	virtual bool DestroyScene(int32 nSceneID);

	virtual void OnSceneCreated( Scene* pScene);
	virtual void OnSceneDestroy( Scene* pScene);

	virtual Scene* GetScene(int32 nSceneID);
	virtual Scene* NextScene(Scene* pScene) { return NULL; }

	void SumSceneCount();

	BornDataMap	m_bornDataMap;

	int32 m_nSceneLoadValue;

	int32 m_nSceneCnt;

	int32 m_nTrunkCnt;

	int32 m_nCopyCnt;
	int32 m_nTeamCopyCnt;
	int32 m_nScenarioCopyCnt;
	int32 m_nGuildCopyCnt;
	int32 m_nTokenCopyCnt;
	int32 m_nTrunkCopyCnt;
	int32 m_nRootCopyCnt;

};

