#pragma once

#include "GameObject.h"
#include "SceneInfo.h"

class SceneCreateArg;

class Scene : public GameObject
{
public:
	Scene(void);
	virtual ~Scene(void);

	virtual void OnCreate( SceneCreateArg& arg);
	virtual bool Tick( int32 nDeltaTime );

	int32 GetSceneID() { return m_nSceneID; }
	void SetSceneID(int32 nID) { m_nSceneID = nID; }

	int32 Instance() { return SceneInfo::GetSceneInstanceID(m_nSceneID); }
	const SceneInfo* GetSceneInfo() const { return m_pSceneInfo; }
	int16 SceneSID() { return m_pSceneInfo->m_SceneSID; }

	int32 GetPlayerCount() const { return m_nPlayerCnt; }
	bool IsPlayersFull() const { return m_nPlayerCnt >= m_pSceneInfo->m_nPlayerMax; }
protected:
	int32		m_nPlayerCnt;

	int32		m_nNodeID;
	int32		m_nSceneID;
	SceneInfo*	m_pSceneInfo;
};

