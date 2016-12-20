#pragma once

#include "MyVector.h"

class SpaceObject
{
public:
	SpaceObject(void);
	virtual ~SpaceObject(void);

	void SetPos(Vector3& pos);
	inline Vector3& GetPos() { return m_vPos; }

	void SetDir(Vector3& pos);
	inline Vector3& GetDir() { return m_vDir; }

	void SetOldPos(Vector3& pos);
	inline Vector3& GetOldPos() { return m_vOldPos; }

	void SetOldDir(Vector3& pos);
	inline Vector3& GetOldDir() { return m_vOldDir; }

protected:
	Vector3 m_vOldDir;
	Vector3 m_vOldPos;

	Vector3 m_vPos;
	Vector3 m_vDir;

	bool m_bIsPosChanged;
};

