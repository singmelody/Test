#pragma once
class CltGameActor : public GameObj, public GameActor, public CommonDataOwner
{
public:
	CltGameActor(void);
	virtual ~CltGameActor(void);

	virtual void SetParamPool(ParamPool* pPool);

	int32 GetAvatarID() { return m_nSrvID; }
	void SetAvatarID(int32 nID) { m_nSrvID = nID; }

	virtual void SetCurPos( Vec3& pos);
	virtual const Vec3& GetCurPos();

	virtual void SetCurDir(Vec3& pos);
	virtual const Vec3 GetCurDir();
protected:
	int32 m_nSrvID;
};

