#include "StdAfx.h"
#include "FSMExtendData.h"
#include "PacketFactory.h"
#include "PacketProcessor.h"
#include "PacketBase.h"

FINISH_FACTORY_ARG0(FSMExtendData);
FSMExtendData::FSMExtendData(void)
{
	m_OwnerID = 0;

	m_nDataCnt = 0;
	m_nDataSize = 0;
}


FSMExtendData::~FSMExtendData(void)
{
}

bool FSMExtendData::SerializeData(uint16 nSyncFlag,  DS::MemoryIOStreamBase& ios)
{
	return true;
}

bool FSMExtendData::FillPacket(class PacketBase& pkt)
{
	return true;
}

void FSMExtendData::ProcessPacket(class PacketProcessor* pProccessor)
{
	char* pBuffer = m_vDataBuffer;

	for (int32 i = 0; i < m_nDataCnt; ++i)
	{
		const int32 nPktID = *((int32*)pBuffer);

		PacketBase* pPkt = (PacketBase*)PacketFactory::Instance().New(nPktID);
		if(!pPkt)
			return;

		pBuffer = pPkt->ReadPacket(pBuffer);
		pProccessor->ProcessPacket(pPkt);

		FACTORY_DEL_PACKET(pPkt);
	}
}

FSMExtendDataList::FSMExtendDataList()
{

}

FSMExtendDataList::~FSMExtendDataList()
{
	for (iterator itr = begin(); itr != end(); ++itr)
	{
		FSMExtendData* pData = *itr;
		FACTORY_DELOBJ(pData);
	}
}
