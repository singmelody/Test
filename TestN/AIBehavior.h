#pragma once

class AIBehavior : public AISerializeBase
{
	friend class AIBehaviorGroup;
	friend class AIBehaviorManager;

	AIUNIT_FACTORY(AIBehavior)
	AIUNIT_Ref( AIBehavior, AISerializeBase)
	Add_Ref_Member( m_nCDGroup, int32, eTB_Flag_Sec, CDGroupId);
	Add_Ref_Member( m_nClear, int32, eTB_Flag_Sec, Clear);
	AIUNIT_REF_END(AIBehavior)
public:
	AIBehavior(void);
	virtual ~AIBehavior(void);

	virtual AIBehavior* Process(NodeAvatar& avatar);
	virtual AIBehavior* Clone();

	AIBehavior* GetBehavior(int32 nIdx);
	int32 GetBehaviorCount();


protected:
	int32 m_nBehCount;
};

