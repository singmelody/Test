#pragma once
#include "scenariomanager.h"
class NodeScenarioManager :
	public ScenarioManager
{
public:
	NodeScenarioManager(void);
	~NodeScenarioManager(void);

	void RegPeerPktHandle( class PacketProcessor* pProc);
protected:
	void PktHintFinish( class PacketHintFinish* pPkt);
};

