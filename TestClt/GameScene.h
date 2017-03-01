#pragma once
#include "BaseType.h"
#include "GameDummy.h"
#include <map>

class GameObj;
class CltGameActor;
class GameLocalPlayer;
class SceneInfo;
class GameActor;

typedef std::map<int32, GameObj*> GameObjMap;

class GameScene
{
public:
	GameScene(void);
	~GameScene(void);

	void Init(int32 nSceneID, int32 nZoneID);

	void Tick(int32 nFrameTime);

	CltGameActor* GetGameActor(int32 nAvatarID);
	GameObj* GetGameObj(int32 nAvatarID);
	GameDummy* GetGameDummy(int32 nAvatarID);

	int32 m_nSceneID;
	int32 m_nSceneSID;

	int32 m_nStartX, m_nStartY;
	int32 m_nSceneSizeX, m_nSceneSizeY, m_nSceneSizeZ;
	int32 m_nZoneID;


	GameLocalPlayer*	m_pLocalPlayer;
	SceneInfo*			m_pSceneInfo;
	GameActor*			m_pSelectActor;

	GameObjMap			m_mapGameActor;
	bool				m_bDrawDirty;
};

