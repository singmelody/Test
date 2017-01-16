#pragma once
#include "CltPktHandleManager.h"

class CltActorPktHandleManager : public CltPktHandleManager, public Singleton<CltActorPktHandleManager>
{
public:
	CltActorPktHandleManager(void);
	virtual ~CltActorPktHandleManager(void);

	virtual void RegisterPktHandlers(class PacketProcessor* pProcessor);
	virtual void RemovePktHandlers(class PacketProcessor* pProcessor);

	virtual void PktNode_InitAvatarData(class PacketInitAvatarData* pPkt);
	virtual void PktNode_UpdateAvatarData(class PacketUpdateAvatarData* pPkt);


};

