#include "StdAfx.h"
#include "NodeServer.h"
#include "WatchDog.h"

NodeServer::NodeServer(void)
{
}


NodeServer::~NodeServer(void)
{
}

bool NodeServer::Init(int32 nArgc, char* argv[])
{

	return true;
}

bool NodeServer::Start()
{
	WatchDog::Instance().RegWatchDog( GAME_THREAD_WATCHDOG_ID, "Node-Game-Thread", NODE_FRAME_TIME*10);
	return StartMainLoop(NODE_FRAME_TIME);
}

void NodeServer::ProcessLogic(int32 nFrameTime)
{
	NodeBase::ProcessLogic(nFrameTime);

	TickEvent();
}
