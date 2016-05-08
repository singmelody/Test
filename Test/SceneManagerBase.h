#pragma once
#include "LoadTemplate.h"
#include <map>

class SceneInfo;

class SceneManagerBase : public LoadTemplate
{
public:
	SceneManagerBase(void);
	virtual ~SceneManagerBase(void);

	const SceneInfo* GetSceneInfo(uint16 nSceneSID);


protected:
	typedef std::map<uint16, SceneInfo> SceneDataMap;
	static SceneDataMap	SceneInfos;
};

