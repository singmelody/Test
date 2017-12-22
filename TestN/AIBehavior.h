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
	Add_Ref_Member( m_Clear, int32, eTB_Flag_Sec, Clear);
	AIUNIT_Ref_End(AIBehavior)
public:
	AIBehavior(void);
	virtual ~AIBehavior(void);

	virtual AIBehavior* Process(NodeAvatar& avatar);
	virtual AIBehavior* Clone();

	AIBehavior* GetBehavior(int32 nIdx);
	int32 GetBehaviorCount();

	void AddBehavior(int32 nIdx, AIBehavior* ptr);

	int32 GetParentID() { return m_nParentID; }
	void SetParentID(int32 nID) { m_nParentID = nID; }

	void SetGroup(AIBehavior* group);
	void SetGroup(int32 nID) { m_nGrpIdx = nID; }
	int32 GetGroupID(){ return m_nGrpIdx; }

	void AddAction(AIBehaviorAction* pAct);

	AIBehaviorAction* GetActionInstance();
	AIBehaviorAction* GetActionbByActIdx(int32 nActIdx);

	int32 GetCDGroupID() { return m_nCDGroupID; }
	void SetCDGroupID(int32 nID) { m_nCDGroupID = nID; }

	bool IsClear() { return m_Clear; }
protected:
	int32 m_nParentID;
	int32 m_nChildIdx;
	int32 m_nGrpIdx;
	
	int32 m_nBehCount;
	int32 m_nCDGroupID;
	bool  m_Clear;

	AIBehaviorAction* m_pAction;
	AIBehavior* m_pBehList[MAX_CHILD_AI_BEHAVIOR_COUNT];
	AIBehaviorGroup* m_pGroup;
};

