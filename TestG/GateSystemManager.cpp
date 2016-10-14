#include "StdAfx.h"
#include "GateSystemManager.h"
#include "MyLog.h"
#include "PacketFactory.h"
#include "PacketBase.h"

GateSystemManager::GateSystemManager(void)
{
}


GateSystemManager::~GateSystemManager(void)
{
}

void GateSystemManager::PrintPackets()
{
	MyLog::message("******************************");

	int32 nCnt = PacketFactory::Instance().GetFactoryCount();

	for (int32 i = 0; i < nCnt; ++i)
	{
		FactoryBase_Arg0* pFac = PacketFactory::Instance().GetFactory(i);
		if(!pFac)
			continue;

		const char* pName = pFac->ClassName();
		int32 nPacketID = -1;
		if( pName != NULL )
			nPacketID = UtilID::CreateFromString(pName);

		PacketBase* pPkt = (PacketBase*)pFac->New();
		if(!pPkt)
			continue;

		if( pFac->ClassSize() != pPkt->PacketClassSize() )
			MyLog::error("!!!!<Packet[%s] id[%d] Size Error>", pName, nPacketID);

		pFac->Free(pPkt);
	}
	 
	MyLog::message("******************************");

}
