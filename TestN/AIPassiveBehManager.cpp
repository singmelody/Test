#include "StdAfx.h"
#include "AIPassiveBehManager.h"


AIPassiveBehManager::AIPassiveBehManager(void)
{
	nLastRst = -1;
	m_pGroup.clear();
}

AIPassiveBehManager::~AIPassiveBehManager(void)
{
	Clear();
}

void AIPassiveBehManager::Clear()
{
	auto itr = m_pGroup.begin();
	for(;itr != m_pGroup.end(); ++itr)
	{
		if(itr->second)
			FACTORY_DELOBJ(itr->second);
	}

	m_pGroup.clear();
}

AIPassiveCondGroup* AIPassiveBehManager::GetPassiveCondGroup(int32 nID)
{
	auto itr = m_pGroup.find(nID);
	if( itr == m_pGroup.end())
		return NULL;

	return itr->second;
}

bool AIPassiveBehManager::Check(NodeAvatar* pAvatar, int32 nType, int32 nSrvAvatarID)
{
	if(!pAvatar->GetParamPool())
		return false;

	int32 nID = PARAM_GET_VALUE( pAvatar->GetParamPool(), aipassivedataid, -1);

	AIPassiveCondGroup* pGroup = GetPassiveCondGroup(nID);
	if(!pGroup)
		return false;

	bool bCheck = pGroup->Check( pAvatar, nType, nSrvAvatarID);
	nLastRst = pGroup->GetLastResult();

	return bCheck;
}

bool AIPassiveBehManager::LoadDataFromDB(DBInterface* ptr)
{
	if(!ptr)
		return false;

	Load_AIPassiveCondGroup(ptr);

	return true;
}
