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

	virtual void OnCreate( SceneCreateArg& arg){}
	virtual bool Tick( int32 nDeltaTime );

	int32 Instance() { return SceneInfo::GetSceneInstanceID(m_nSceneID); }
	const SceneInfo* GetSceneInfo() const { return m_pSceneInfo; }
	int16 SceneSID() { return m_pSceneInfo->m_nSceneSID; }

	int32 GetPlayerCount() const { return m_nPlayerCnt; }

	bool IsPlayersFull() const { return m_nPlayerCnt >= m_pSceneInfo->m_nPlayerMax; }
	bool IsPlayersCrowded();

	inline bool IsCreating() { return m_nSceneState == eSceneState_Creating; }
	inline bool IsRunning() { return m_nSceneState == eSceneState_Running; }
	inline bool IsWaitClosing() { return m_nSceneState == eSceneState_WaitClosing; }
	inline bool IsClosing() { return m_nSceneState == eSceneState_Closing; }

	int32 GetLoadValue();

	int32 GetSceneID() { return m_nSceneID; }
	void SetSceneID(int32 nID) { m_nSceneID = nID; }

	int32 InstanceID() { return SceneInfo::GetSceneInstanceID(m_nSceneID); }

	int32 GetNodeID() { return m_nNodeID; }
	void SetNodeID(int32 nID) { m_nNodeID = nID; }

	void SetArenaID(int32 nArenaID) { m_nArenaID = nArenaID; }
	int32 GetArenaID() { return m_nArenaID; }

	void SetSceneLevel(int32 nSceneLv) { m_nSceneLv = nSceneLv; }
	int32 GetSceneLevel() { return m_nSceneLv; }

	uint64 GetRequestTime() { return m_nRequestTime; }
	void SetRequestTime(uint64 time) { m_nRequestTime = time; }
	uint64 m_nRequestTime;

	virtual bool SetProcessBits( int64 nProcessBits);

	inline void AddPlayerCount(int32 nNum)
	{
		m_nPlayerCnt += nNum;
		OnPlayerChanged();
	}

	virtual void OnPlayerChanged(){}

	inline void SetSceneState(SceneState state) { m_nSceneState = state; }
	
	SceneState  m_nSceneState;

	int32		m_nPlayerCnt;

	int32		m_nNodeID;
	int32		m_nSceneID;
	SceneInfo*	m_pSceneInfo;

	int32		m_nArenaID;
	int32		m_nSceneLv;

	int64		m_nSceneProcessBits;
	int64		m_nSceneInstanceBits;
};

