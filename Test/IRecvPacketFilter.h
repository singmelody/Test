#pragma once


class PacketBase;

struct IRecvPacketFilter
{
	virtual ~IRecvPacketFilter() {}

	virtual bool Thread_CheckPacketTypeValid( PacketBase& pkt) = 0;
};