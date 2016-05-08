#pragma once
class SceneInfo
{
public:
	SceneInfo(void);
	virtual ~SceneInfo(void);

	static uint16 GetSceneSID(uint32 nSceneID);
	static uint16 GetSceneInstanceID(uint32 nSceneID);

	static const SceneInfo* GetSceneInfo( uint16 nSceneSID);
};

