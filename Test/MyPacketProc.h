#pragma once

#include "PacketProcessor.h"

class MyPacketProc : public PacketProcessor
{
public:
	MyPacketProc(void);
	virtual ~MyPacketProc(void);

	virtual int32 DoProc();
};

