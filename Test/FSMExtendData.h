#pragma once
#include "PacketData.h"
#include "Factory.h"
#include <list>

class FSMExtendData : public PacketData
{
	DECLARE_FACTORY_ARG0( FSMExtendData, -1, new PoolAllocatorEx)
public:
	FSMExtendData(void);
	~FSMExtendData(void);

	virtual bool SerializeData(uint16 nSyncFlag, class MemoryIOStreamBase& ios);

	int32	m_OwnerID;

	uint8	m_nDataCnt;
	uint16	m_nDataSize;
	char	m_vDataBuffer[512];

	bool	FillPacket( class PacketBase& pkt);
	void	ProcessPacket(class PacketProcessor* pProccessor);
};

class FSMExtendDataList : public std::list<FSMExtendData*>
{
public:
	FSMExtendDataList();
	virtual ~FSMExtendDataList();
};