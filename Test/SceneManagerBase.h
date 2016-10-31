#pragma once
#include "LoadTemplate.h"
#include <map>

class SceneInfo;

class SceneManagerBase : public LoadTemplate
{
public:
	SceneManagerBase(void);
	virtual ~SceneManagerBase(void);

	static const SceneInfo* GetSceneInfo(uint16 nSceneSID);
	void ReleaseSceneInfos();

protected:
	typedef std::map<uint16, SceneInfo*> SceneDataMap;
	static SceneDataMap	SceneInfos;
};

