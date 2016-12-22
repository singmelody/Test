#pragma once
class AIBehaviorSelector : public AIBehavior
{
	AIUNIT_FACTORY(AIBehaviorSelector)
	AIUNIT_Ref(AIBehaviorSelector, AIBehavior)
public:
	AIBehaviorSelector(void);
	~AIBehaviorSelector(void);
};

