#pragma once

#include "AIBehavior.h"
#include "AITypeDefine.h"

class AIBehaviorSelector : public AIBehavior
{
	AIUNIT_FACTORY(AIBehaviorSelector)
	AIUNIT_Ref(AIBehaviorSelector, AIBehavior)
	Add_Ref_Member( m_bUseWeight, int32, eTB_Flag_Sec, logic)
	Add_Ref_Member( m_nCDGroupID, int32, eTB_Flag_Sec, logic)
	AIUNIT_Ref_End(AIBehaviorSelector)
public:
	struct sAIConditionUnit
	{
		AICondition* pCondition;
		AIBehaviorWeight sWeight;
		sAIConditionUnit() : pCondition(0) {}
	};

	typedef std::vector<sAIConditionUnit> ConditionList;

	AIBehaviorSelector(void);
	virtual ~AIBehaviorSelector(void);

	virtual AIBehavior* Process(NodeAvatar& avatar);
	virtual AIBehavior* Clone();
	void AddCondition(int32 nIdx, AICondition* pCondition);
	void SetBehaviorWeight(int32 nConditionIdx, int32 nBehIdx, int32 nWeight);
	int32 GetConditionCount() { return m_nConditionCount; }

	AICondition* GetCondition(int32 nIdx);
	void LoadWeightString(int32 nIdx, const char* sWeight);
protected:
	bool	m_bUseWeight;
	int32	m_nConditionCount;

	static const MAX_AI_CONDITION_SIZE = 64;
	sAIConditionUnit m_ConditionList[MAX_AI_CONDITION_SIZE];
};

class AIBehaviorTreeGroup : public AIBehavior
{
	friend class AIBehaviorManager;

	AIUNIT_FACTORY(AIBehaviorTreeGroup)
	AIUNIT_Ref( AIBehaviorTreeGroup, AIBehavior)
	Add_Ref_Member( m_nAITreeID, int32, eTB_Flag_Sec, nAITreeID)
	Add_Ref_Member( m_nAIDataID, int32, eTB_Flag_Sec, nAIDataID)
	AIUNIT_Ref_End(AIBehaviorTreeGroup)

public:
	AIBehaviorTreeGroup();
	virtual ~AIBehaviorTreeGroup();

	virtual AIBehavior* Process(NodeAvatar& avatar);
	virtual AIBehavior* Clone();

	void SetAIGroup( class AIBehaviorGroup* pGroup) { m_pGroup = pGroup; }
protected:
	int32 m_nAITreeID;
	int32 m_nAIDataID;
};

