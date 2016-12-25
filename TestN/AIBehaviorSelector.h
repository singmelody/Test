#pragma once

#include "AIBehavior.h"
#include "AITypeDefine.h"

class AIBehaviorSelector : public AIBehavior
{
	AIUNIT_FACTORY(AIBehaviorSelector)
	AIUNIT_Ref(AIBehaviorSelector, AIBehavior)
public:
	AIBehaviorSelector(void);
	~AIBehaviorSelector(void);

protected:
	bool	m_bUseWeight;
	int32	m_nConditionCount;
};

