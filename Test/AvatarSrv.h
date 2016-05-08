#pragma once

#include "GameObject.h"
#include "PacketSender.h"
#include "Avatar.h"
#include "MyPacket.h"

class AvatarSrv : public GameObject, public virtual PacketSender, public virtual Avatar
{
	DECLARE_FACTORY_ARG0( AvatarSrv, -1, new PoolAllocator)
public:
	AvatarSrv(void);
	virtual ~AvatarSrv(void);

	int32 GetWorldSrvID() { return m_nWorldID; }
	void SetWorldSrvID( int32 nID) { m_nWorldID = nID; }

	int32 GetNodeSrvID() { return m_nNodeID; }
	void SetNodeSrvID(int32 nID) { m_nNodeID = nID; }

	int32 GetGateSrvID() { return m_nGateSrvID; }
	void SetGateSrvID(int32 nID) { m_nGateSrvID = nID; }

	void SetPendingTime(int32 nTime) { m_nPendingTime = nTime; }
	int32 GetPendingTime() { return m_nPendingTime; }
	int32 SubPendingTime(int32 nDeltaTime) { return m_nPendingTime -= nDeltaTime; }
protected:
	int32	m_nGateSrvID;
	int32	m_nGateChannelID;

	int32	m_nNodeID;
	int32	m_nWorldID;
	int32	m_nPendingTime;
};

