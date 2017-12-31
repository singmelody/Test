#pragma once

#include "EventProcessorBase.h"
#include "NodeScene_CreateDestroy.h"
#include "ScenarioMgr.h"

class NodeScene : public NodeScene_CreateDestroy, public EventProcessorBase<NodeScene>
{
public:
	NodeScene(void);
	~NodeScene(void);

	bool DebugTick(int32 nDeltaTime);

	void ExitSceneAndDestroy(class NodeNPC* pNpc);
protected:
	int64 m_nAreaEnableMask;
	int32 m_nFastTickTime;
	int32 m_nNormalTickTime;

	ScenarioMgr	m_mgrScenarios;
};

