#pragma once
#include "MyCltManager.h"
#include "PacketBase.h"
#include "MyClt.h"
#include "GameObj.h"

class CltPktHandleManager
{
public:
	CltPktHandleManager(void);
	virtual ~CltPktHandleManager(void);

	MyClt* GetClient(PacketBase* pPkt);
	GameObj* GetGameObj(PacketBase* pPkt);

	MyCltManager*	m_pMyCltManager;
};

