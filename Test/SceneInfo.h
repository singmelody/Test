#pragma once

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
class SceneInfo
{
public:
	SceneInfo(void);
	virtual ~SceneInfo(void);

	static uint16 GetSceneSID(uint32 nSceneID);
	static uint16 GetSceneInstanceID(uint32 nSceneID);

	static const SceneInfo* GetSceneInfo( uint16 nSceneSID);

	uint16			m_SceneSID;
	std::string		m_SceneName;
	int32			m_ShowDescript;
	int32			m_ShowName;
	bool			m_bDynEnterSwitch;
	int32			m_nNpcAutoLevelStep;

	uint8			m_CreateType;
};

