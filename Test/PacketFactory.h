#pragma once

#include "Singleton.h"
#include "FactoryManagerBase.h"

#define FACTORY_NEW_PACKET( PACKET_CLASS ) \
	(PACKET_CLASS*)PacketFactory::Instance().FactoryManagerBase::New(#PACKET_CLASS);

#define FACTORY_DEL_PACKET(pPkt) \
	{if(pPkt) { PacketFactory::Instance().Delete(pPkt); pPkt = NULL; } }

class PacketFactory : public FactoryManagerBase, public Singleton<PacketFactory>
{
public:
	PacketFactory(void);
	~PacketFactory(void);

	FactoryBase_Arg0* GetFactoryByPacketID(int32 nPacketID)
	{
		auto itr = m_FuncName_Arg0.find(nPacketID);
		if(itr == m_FuncName_Arg0.end())
			return NULL;

		return itr->second;
	}

	void* New(int32 nPacketID)
	{
		FactoryBase_Arg0* pFactory = GetFactoryByPacketID(nPacketID);
		if(!pFactory)
			return NULL;

		return pFactory->New();
	}

	void Delete(void* ptr)
	{
		FactoryManagerBase::Delete(ptr);
	}
};

