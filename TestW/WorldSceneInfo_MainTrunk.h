#pragma once

#include "DBInterface.h"
#include "WorldSceneInfo.h"

class WorldAvatar;

class WorldSceneInfo_MainTrunk : public WorldSceneInfo_Trunk
{
public:
	WorldSceneInfo_MainTrunk(void);
	~WorldSceneInfo_MainTrunk(void);

	virtual bool LoadSceneInfo(DBRow& row);

	virtual bool TryEnterTargetScene( WorldAvatar* pAvatar, int16 nInstanceID, int32& failreason);
};

