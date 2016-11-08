#pragma once

#include "BaseType.h"
class PacketData
{
public:
	PacketData(void);
	virtual ~PacketData(void);

	virtual bool SerializeData( uint16 nSyncFlag, class MemoryIOStreamBase& ios){ return false; }
};

