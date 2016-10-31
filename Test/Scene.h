#pragma once

#include "GameObject.h"
#include "SceneInfo.h"

class SceneCreateArg;
class SceneInfo;

class Scene : public GameObject
{
public:
	enum SceneState
	{
		eSceneState_Creating	= 0,
		eSceneState_Running		= 1,
		eSceneState_WaitClosing = 2,
		eSceneState_Closing		= 3,
	};

	Scene(void);
	virtual ~Scene(void);

	virtual void OnCreate( SceneCreateArg& arg);
	virtual bool Tick( int32 nDeltaTime );

	int32 InstanceID() { return SceneInfo::GetSceneInstanceID(m_nSceneID); }

	int32 GetSceneID() { return m_nSceneID; }
	void SetSceneID(int32 nID) { m_nSceneID = nID; }

	int32 Instance() { return SceneInfo::GetSceneInstanceID(m_nSceneID); }
	const SceneInfo* GetSceneInfo() const { return m_pSceneInfo; }
	int16 SceneSID() { return m_pSceneInfo->m_SceneSID; }

	int32 GetPlayerCount() const { return m_nPlayerCnt; }
	bool IsPlayersFull() const { return m_nPlayerCnt >= m_pSceneInfo->m_nPlayerMax; }

	inline bool IsCreating() { return m_nSceneState == eSceneState_Creating; }
	inline bool IsRunning() { return m_nSceneState == eSceneState_Running; }
	inline bool IsWaitClosing() { return m_nSceneState == eSceneState_WaitClosing; }
	inline bool IsClosing() { return m_nSceneState == eSceneState_Closing; }

	int32 GetLoadValue();

	void SetArenaID(int32 nArenaID) { m_nArenaID = nArenaID; }
	int32 GetArenaID() { return m_nArenaID; }

	void SetSceneLevel(int32 nSceneLv) { m_nSceneLv = nSceneLv; }
	int32 GetSceneLevel() { return m_nSceneLv; }
	
	SceneState  m_nSceneState;

	int32		m_nPlayerCnt;

	int32		m_nNodeID;
	int32		m_nSceneID;
	SceneInfo*	m_pSceneInfo;

	int32		m_nArenaID;
	int32		m_nSceneLv;
};

