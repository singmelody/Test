#pragma once
class AIBehaviorWeight
{
public:
	AIBehaviorWeight(void);
	~AIBehaviorWeight(void);

	void Init()
	{
		for (int32 i = 0; i < MAX_CHILD_AI_BEHAVIOR_COUNT; ++i)
		{
			nWeights[i] = 0;
		}
	}

	AIBehaviorWeight operator + (const AIBehavior& w2)
	{
		AIBehaviorWeight weight;

		for (int32 i = 0; i < MAX_CHILD_AI_BEHAVIOR_COUNT; ++i)
		{
			if( nWeights[i] == -1 || w2.Weights[i] == -1)
				weight.Weights[i] = -1;
			else
				weight.Weights[i] = Weights[i] + w2.Weights[i];
		}

		return weight;
	}

	AIBehaviorWeight& operator += (const AIBehaviorWeight& w2)
	{
		for (int32 i = 0; i < MAX_CHILD_AI_BEHAVIOR_COUNT; ++i)
		{
			if( nWeights[i] == -1 ||  w2.Weights[i] == -1)
				nWeights[i] = -1;
			else
				nWeights[i] += w2.nWeights[i];
		}

		return *this;
	}

	AIBehaviorWeight& operator = (const AIBehaviorWeight& w2)
	{
		for (int32 i = 0; i < MAX_CHILD_AI_BEHAVIOR_COUNT; ++i)
		{
			nWeights[i] = w2.nWeights[i];
		}

		return *this;
	}
};


