#pragma once

#include "LoadTemplate.h"
#include "Singleton.h"
#include "NodeAvatar.h"

enum AIPassiveType
{
	eAIPassive_PreValueDamage = 0,
	eAIPassive_PreStateDamage = 1,
	eAIPassive_AfterStateDamge= 2,
	eAIPassive_MaxNum,
};

typedef std::map<int32, AIPassiveCondGroup*> AIPassiveGroupMap;
class AIPassiveBehManager : public LoadTemplate, public Singleton<AIPassiveBehManager>
{
public:
	AIPassiveBehManager(void);
	virtual ~AIPassiveBehManager(void);

	virtual bool LoadDataFromDB(DBInterface* ptr);
	void Clear();
	bool Check(NodeAvatar* pAvatar, int32 nType, int32 nSrvAvatarID);
	int32 GetLastResult() { return nLastRst; }

protected:
	void Load_AIPassiveCondGroup(DBInterface* pDBI);

	int32 nLastRst;
	AIPassiveGroupMap	m_pGroup;
};

