#pragma once
#include "Singleton.h"
#include "DBAFuncManager.h"

class DBAGuidManager : public DBAFuncManager, public Singleton<DBAGuidManager>
{
public:
	DBAGuidManager(void);
	~DBAGuidManager(void);

	void RegPeerPktHandle(class PacketProcessor* pProc);
};

