#pragma once
#include "AISerializeBase.h"

#define MAX_CHILD_AI_BEHAVIOR_COUNT 15

class AIBehavior : public AISerializeBase
{
	friend class AIBehaviorGroup;
	friend class AIBehaviorManager;

	AIUNIT_FACTORY(AIBehavior)
	AIUNIT_Ref( AIBehavior, AISerializeBase)
	Add_Ref_Member( m_nCDGroupID, int32, eTB_Flag_Sec, CDGroupId);
	Add_Ref_Member( m_bClear, int32, eTB_Flag_Sec, Clear);
	AIUNIT_Ref_End(AIBehavior)
public:
	AIBehavior(void);
	virtual ~AIBehavior(void);

	virtual AIBehavior* Process(NodeAvatar& avatar);
	virtual AIBehavior* Clone();

	AIBehavior* GetBehavior(int32 nIdx);
	int32 GetBehaviorCount();


protected:
	int32 m_nParentID;
	int32 m_nChildIdx;
	int32 m_nGrpIdx;
	
	int32 m_nBehCount;
	int32 m_nCDGroupID;
	bool  m_bClear;

	AIBehaviorAction* m_pAction;
	AIBehavior* m_pBehList[MAX_CHILD_AI_BEHAVIOR_COUNT];
	AIBehaviorGroup* m_pGroup;
};

