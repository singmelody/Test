#include "PacketBase.h"

char* PacketBase::ReadPacket(char* buffer)
{
	if(!buffer)
		return NULL;

	char* tmp = buffer;

	m_PacketID = *((int32*)tmp);
	tmp += sizeof(int32);

	uint32 nOff = 0;
	DS::Serialize( m_AvatarID, tmp, nOff, false);
	tmp += nOff;

	if(IsUseIndex())
	{
		m_nIdx = *((uint16*)tmp);
		tmp += sizeof(m_nIdx);
	}

	m_SendType = *((uint8*)tmp);
	return tmp + sizeof(uint8);
}

char* PacketBase::WritePacket( char* pBuffer)
{
	char* tmp = buffer;

	*((int32)*tmp) = m_PacketID;
	tmp += sizeof(int32);

	uint32 nOff = 0;
	DS::Serialize( m_AvatarID, tmp, nOff, true);
	tmp += nOff;

	if(IsUseIndex())
	{
		*((uint16*)tmp) = m_nIdx;
		tmp += sizeof(m_nIdx);
	}

	*((uint8*)tmp) = m_SendType;
	return tmp + sizeof(uint8);
}


//-----------------------------------------
// PacketPackBase
//-----------------------------------------
PacketPackBase::PacketPackBase()
{
	m_PacketCnt = 0;
	m_ContentSize = 100;

	memset( m_pPacket, 0, sizeof(PacketBase*) * MaxCnt);
	m_pSyncPacketArg = NULL;
}

PacketPackBase::~PacketPackBase()
{
	Reset();
	FACTORY_DELOBJ( m_pSyncPacketArg );
}

char* PacketPackBase::ReadPacket(char* buffer)
{

}

char* PacketPackBase::WritePacket(char* buffer)
{

}

bool PacketPackBase::AddupPacket(PacketBase* pPkt)
{

}

void PacketPackBase::Reset()
{
	if( m_PacketCnt >= 0 && m_PacketCnt <= MaxCnt)
	{
		for (int32 i = 0; i < m_PacketCnt; ++i)
		{
			PacketBase*& pPkt = m_pPacket[i];
			if(!pPkt)
			{
				PacketFactory::Instance().Delete( pPkt );
				pPkt = NULL;
			}
		}
	}

	m_PacketCnt = 0;
	m_ContentSize = 100;
}

void PacketPackBase::SetDBASyncArg(PacketSender* pSender)
{

}

void PacketPackBase::SetWorldSyncArg(PacketSender* pSender)
{

}

void PacketPackBase::SetGateSyncArg(PacketSender* pSender, CasterTrunkGate* pTrunk)
{

}

void PacketPackBase::SetNodeSyncArg(PacketSender* pSender, int32 nNodeSrvID)
{

}

void PacketPackBase::SetCltSyncArg(PacketSender* pSender)
{

}
