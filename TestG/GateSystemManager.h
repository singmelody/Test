#pragma once

#include "GateFuncManager.h"
#include "Singleton.h"
#include <vector>

class GateSystemManager : public GateFuncManager, public Singleton<GateSystemManager>
{
public:
	GateSystemManager(void);
	~GateSystemManager(void);

	void PrintPackets();

	void Tick(int32 nFrameTime);
	void ProcGateStressInfo(int32 nFrameTime);

protected:
	int32 m_nLastSocketCnt;
	std::vector<std::string> _blocklist;
};

