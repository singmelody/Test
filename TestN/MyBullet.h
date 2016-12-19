#pragma once

#include "Factory.h"
#include "MyMath.h"

class MyBullet
{
	DECLARE_FACTORY_ARG0( MyBullet, -1, new PoolAllocator);
public:
	MyBullet(void);
	virtual ~MyBullet(void);

	virtual bool Init();
	virtual bool Tick(int32 nDeltaTime);
	virtual void TickAttack();
	virtual void EndTick();
	virtual bool DoDamage(int32 nDeltaTime);

	virtual void DoBomb(Vec3 vPos = Vec3(0,0,0));

	void SetOwner(int32 nOwnerID) { m_nOwnerID = nOwnerID; }
	int32 GetOwner() const { return m_nOwner;}
};

