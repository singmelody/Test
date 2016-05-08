#include "StdAfx.h"
#include "SceneInfo.h"
#include "SceneManagerBase.h"

SceneInfo::SceneInfo(void)
{
}


SceneInfo::~SceneInfo(void)
{
}

uint16 SceneInfo::GetSceneSID(uint32 nSceneID)
{
	nSceneID >>= 16;
	return uint16(nSceneID);
}

uint16 SceneInfo::GetSceneInstanceID(uint32 nSceneID)
{
	nSceneID &= 0xffff;
	return uint16(nSceneID);
}

const SceneInfo* SceneInfo::GetSceneInfo(uint16 nSceneSID)
{
	return SceneManagerBase::GetSceneInfo( nSceneSID );
}
