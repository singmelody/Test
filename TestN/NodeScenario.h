#pragma once
#include "Scenario.h"

class NodeScenario : public Scenario
{
public:
	NodeScenario(void);
	~NodeScenario(void);

protected:

	UID2AvatarIDMap* m_pMapUID2AvatarID2;
};

