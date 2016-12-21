#pragma once
#include "TreeNode.h"

class LogicTreeBase : public TreeNode
{
public:
	LogicTreeBase(void)
	: m_bActive(true)
	, m_bChildFirst(false)
	, m_pOwnerGroup(NULL)
	, m_nNodeID(-1)
	, m_bSelfFinish(false)
	, m_bChildFinish(false)
	, _nextLogic(NULL)
	, _prevLogic(NULL)
	, m_nLogicID(-1)
	, pList(NULL)
	, m_LoadVersion(0)
	{
		memset(m_InputInfo, -1, sizeof(sLogicInputInfo)*LOGIC_MAX_INPUT);
	}
	virtual ~LogicTreeBase(void);

	void DoProcess(); 
	void AddChildLogic(LogicTreeBase* pChild);
	void RemovedChildLogic(LogicTreeBase* pChild);

	void ProcessSelf();
	void ProcessChild();

protected:
	bool m_bActive;
	bool m_bSelfFinish;

	LogicTreeBase*			_nextLogic;
	LogicTreeBase*			_prevLogic;
	MyList<LogicTreeBase*>	pList;
	sLogicInputInfo			m_inputInfo[LOGIC_MAX_INPUT];
};

