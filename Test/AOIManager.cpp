#include "StdAfx.h"
#include "AOIManager.h"


void Classifier::Sort(const Vec3& vCenter)
{
	std::vector<AOIObj*> vObjs;

	// 
	for (auto itr = m_mapAllObjs.begin(); itr != m_mapAllObjs.end(); ++itr)
	{
		AOIObj* pObj = itr->second;
		pObj->m_DistanceSqr = vCenter.GetSquartDistance( pObj->m_FSMBasicData.CurPos );
		vObjs.push_back(pObj);
	}

	std::sort( vObjs.begin(), vObjs.end(), AOIObjCompare());
}

AOIManager::AOIManager(void)
{
	m_fEllapseTime = 0;
}


AOIManager::~AOIManager(void)
{
}

void AOIManager::SetMaxVisibleCount(size_t nMaxPlayer, size_t nMaxNpc)
{

}

void AOIManager::Tick(float fDeltaTime, float fMinMoveDist /*= 4.0f*/, float fMinTimeDelta /*= 1.0f*/)
{
	m_fEllapseTime  += fDeltaTime;

	f32 fDistanceSq = m_vCenterOld.GetSquaredDistance2D( m_vCenterNew );
	if( fDistanceSq < fMinMoveDist && m_fEllapseTime < fMinTimeDelta)
		return;

	m_vCenterOld = m_vCenterNew;
	m_fEllapseTime = 0;

	m_ClassifierNPC.Sort( m_vCenterOld );
	m_ClassifierPlayer.Sort( m_vCenterOld );
}
