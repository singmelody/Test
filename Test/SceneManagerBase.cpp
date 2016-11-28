#include "StdAfx.h"
#include "SceneManagerBase.h"


SceneManagerBase::SceneDataMap SceneManagerBase::SceneInfos;

SceneManagerBase::SceneManagerBase(void)
{
}


SceneManagerBase::~SceneManagerBase(void)
{
	ReleaseSceneInfos();
}

bool SceneManagerBase::LoadDataFromDB(DBInterface* ptr)
{
	{
		
	}

	return true;
}

const SceneInfo* SceneManagerBase::GetSceneInfo(uint16 nSceneSID)
{
	auto itr = SceneInfos.find(nSceneSID);
	if(itr == SceneInfos.end())
		return NULL;

	SceneInfo* pInfo = itr->second;
	return pInfo;
}

void SceneManagerBase::ReleaseSceneInfos()
{
	for (auto itr = SceneInfos.begin(); itr != SceneInfos.end(); ++itr)
	{
		SAFE_DELETE(itr->second);
	}

	SceneInfos.clear();
}
