#include "StdAfx.h"
#include "WorldSceneInfo_MainTrunk.h"
#include "WorldAvatar.h"
#include <sstream>

WorldSceneInfo_MainTrunk::WorldSceneInfo_MainTrunk(void)
{
}


WorldSceneInfo_MainTrunk::~WorldSceneInfo_MainTrunk(void)
{
}

bool WorldSceneInfo_MainTrunk::LoadSceneInfo(DBRow& row)
{
	if( !WorldSceneInfo_Trunk::LoadSceneInfo(row))
		false;

	std::stringstream ss;
	ss<<"maintrunk"<<m_nSceneSID;

	ConfigManager::GetConfigValue( "SceneConfig/ParalletCnt", ss.str(), m_nParalleCnt);
	return true;
}

bool WorldSceneInfo_MainTrunk::TryEnterTargetScene(WorldAvatar* pAvatar, int16 nInstanceID, int32& failreason)
{

}
