#pragma once

#include "Singleton.h"
#include "NodeBase.h"
#include "EventProcessorBase.h"

class NodeServer : public NodeBase, public EventProcessorBase<NodeServer>, public Singleton<NodeServer>
{
public:
	NodeServer(void);
	virtual ~NodeServer(void);

	virtual bool Init(int32 nArgc, char* argv[]);
	bool Start();
	virtual void Exit();

	virtual void ProcessLogic(int32 nFrameTime);
};

