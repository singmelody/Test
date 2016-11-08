#include "StdAfx.h"
#include "FSMExtendData.h"

FINISH_FACTORY_ARG0(FSMExtendData);
FSMExtendData::FSMExtendData(void)
{
}


FSMExtendData::~FSMExtendData(void)
{
}

bool FSMExtendData::SerializeData(uint16 nSyncFlag, class MemoryIOStreamBase& ios)
{
	return true;
}

bool FSMExtendData::FillPacket(class PacketBase& pkt)
{
	return true;
}

void FSMExtendData::ProcessPacket(class PacketProcessor* pProccessor)
{

}
