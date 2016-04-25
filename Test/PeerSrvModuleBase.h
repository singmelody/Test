#pragma once
#include "PeerModuleBase.h"
#include "SrvBase.h"

class PeerSrvModuleBase : public PeerModuleBase, public SrvBase
{
public:
	PeerSrvModuleBase(void);
	~PeerSrvModuleBase(void);

	virtual bool Init(int32 nArgc, char* argv[]);
	virtual void Exit();

	virtual bool ProcessPacket();

protected:
	f32		m_fSrv
};

