#include "StdAfx.h"
#include "NodeSrvGrp.h"
#include <algorithm>

NodeSrvInfo::NodeSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& addr)
	: ServerInfo( nSrvType, nSrvID, nSocketID, addr)
{
	nTotalLoadValue = 0;
	nTrunkLoadValue = 0;
}

void NodeSrvInfo::UpdateDetailsPool()
{
	ParamPool*& pPool = m_pParamDetails;
	if(!pPool)
	{
		int32 nIdx = ServerManager::GetSrvIndex( nSrvID );
		pPool = ParamSet::CreateNew( eParam_WorldNodeDetails, nIdx);
	}

	PARAM_SET_VALUE( pPool, nSrvID, nSrvID, true);
	PARAM_SET_VALUE( pPool, SceneLoadValue, nTotalLoadValue, true);
}

void NodeSrvInfo::AddupSceneLoadValue(Scene* pScene, bool bAdd)
{
	int32 nValue = pScene->GetLoadValue();
	if(!bAdd)
		nValue = 0;

	nTotalLoadValue += nValue;

	const SceneInfo* pInfo = pScene->GetSceneInfo();
	if(pInfo->IsMainTrunk())
		nTrunkLoadValue += nValue;

	if( m_pParentGrp )
		m_pParentGrp->SortList();
}


void NodeSrvGrp::SortList()
{
	std::sort( m_listByTrunkValue.begin(), m_listByTrunkValue.end(), TrunkCmp());
	std::sort( m_listByLoadValue.begin(), m_listByLoadValue.end(), LoadValueCmp());
}

int32 NodeSrvGrp::GetLowSceneLoadNode(int32 exclude_ID /* = SERVERID_NULL */)
{
	int32 nMinLoadValue = 0;
	int32 nNodeID = SERVERID_NULL;

	for (auto itr = m_SrvMap.begin(); itr != m_SrvMap.end(); ++itr)
	{
		NodeSrvInfo* pInfo = (NodeSrvInfo*)itr->second;
		if( nNodeID == SERVERID_NULL || pInfo->nTotalLoadValue < nMinLoadValue )
		{
			nNodeID = pInfo->nSrvID;
			nMinLoadValue = pInfo->nTotalLoadValue;
		}
	}

	return nNodeID;
}