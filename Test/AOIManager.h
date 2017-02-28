#pragma once

class AOIObj
{
	DECLARE_FACTORY_ARG0( AOIObj, -1, new PoolAllocator);
public:
	AOIObj();
	virtual ~AOIObj();

	void UpdateDistacenSqr(const Vec3& vCenter);

	FSMBasicData m_FSMBasicData;
	f32 m_DistanceSqr;
};

class AOIObjCompare
{
public:
	bool operator()(AOIObj* pLeft, AOIObj* pRight) const
	{
		return (pLeft->m_DistanceSqr < pRight->m_DistanceSqr);
	}
};

typedef ID2ItemMap<int32,AOIObj> AOIObjMap;


class Classifier
{
public:
	Classifier();

	void Sort( const Vec3& vCenter);

	AOIObjMap	m_mapAllObjs;

};

class AOIManager
{
public:
	AOIManager(void);
	~AOIManager(void);

	void SetMaxVisibleCount(size_t nMaxPlayer, size_t nMaxNpc);
	void Tick(float fDeltaTime, float fMinMoveDist = 4.0f, float fMinTimeDelta = 1.0f);

	ClassifierPlayer	m_ClassifierPlayer;
	ClassifierNPC		m_ClassifierNPC;

	Vec3				m_vCenterNew;
	Vec3				m_vCenterOld;
protected:
	float m_fEllapseTime;
};

