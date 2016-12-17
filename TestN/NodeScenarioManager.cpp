#include "StdAfx.h"
#include "NodeScenarioManager.h"


NodeScenarioManager::NodeScenarioManager(void)
{
}


NodeScenarioManager::~NodeScenarioManager(void)
{
}

void NodeScenarioManager::RegPeerPktHandle(class PacketProcessor* pProc)
{
	//REG_PACKET_HANDLER( pProc, PacketHintFinish, NodeScenario, PktHintFinish);
}

void NodeScenarioManager::PktHintFinish(class PacketHintFinish* pPkt)
{
	return;
}
