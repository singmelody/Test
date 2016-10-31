#include "StdAfx.h"
#include "SceneManagerBase.h"


SceneManagerBase::SceneManagerBase(void)
{
}


SceneManagerBase::~SceneManagerBase(void)
{
	ReleaseSceneInfos();
}

const SceneInfo* SceneManagerBase::GetSceneInfo(uint16 nSceneSID)
{
	return SceneManagerBase::GetSceneInfo(nSceneSID);
}

void SceneManagerBase::ReleaseSceneInfos()
{
	for (auto itr = SceneInfos.begin(); itr != SceneInfos.end(); ++itr)
	{
		SAFE_DELETE(itr->second);
	}

	SceneInfos.clear();
}
