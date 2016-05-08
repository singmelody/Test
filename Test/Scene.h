#pragma once

#include "GameObject.h"

class SceneCreateArg
{
public:
	SceneCreateArg();

	int16	m_SceneSID;
	int32	m_SceneID;
	int32	m_SceneLevel;
	int32	m_NodeSrvID;
	int32	m_ArenaID;

	int64	m_nSceneProcessBits;
	int64	m_nSceneCustomData;
	uint64	m_nRequestTime;

	int16	TemplateID();
	int16	InstanceID();
};

class Scene : public GameObject
{
public:
	Scene(void);
	virtual ~Scene(void);

	virtual void OnCreate( SceneCreateArg& arg);
	virtual bool Tick( int32 nDeltaTime );
};

