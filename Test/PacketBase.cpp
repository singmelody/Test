#include "StdAfx.h"
#include "PacketBase.h"
#include "PacketFactory.h"
#include "DataSerialization.h"
#include "MyClass.h"
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
	if(!buffer)
		return NULL;

	char* tmp = PacketBase::ReadPacket(buffer);

	m_PacketCnt = *((int8*)tmp);
	tmp += sizeof(int8);

	if ( m_PacketCnt >= 0 && m_PacketCnt <= MaxCnt)
	{
		for (int32 i = 0; i < m_PacketCnt; ++i)
		{
			int32 nPacketID = *((int32*)tmp);

			if(nPacketID == 0)
				continue;

			PacketBase* pPkt = (PacketBase*)PacketFactory::Instance().New(nPacketID);
			if(!pPkt)
				return tmp;

			tmp = pPkt->ReadPacket( tmp );

			m_pPacket[i] = pPkt;
		}
	}
	else
	{
		m_PacketCnt = 0;
	}

	return tmp;
}

char* PacketPackBase::WritePacket(char* buffer)
{
	char* tmp = PacketBase::WritePacket(buffer);

	*((int8*)tmp) = m_PacketCnt;
	tmp += sizeof(int8);

	if( m_PacketCnt >= 0 && m_PacketCnt <= MaxCnt )
	{
		for (int32 i = 0; i < m_PacketCnt; ++i)
		{
			PacketBase* pPkt = m_pPacket[i];
			if(!pPkt)
			{
				*((int32*)tmp) = 0;
				tmp += sizeof(int32);
				continue;
			}

			tmp = pPkt->WritePacket(tmp);
		}
	}

	return tmp;
}

bool PacketPackBase::AddupPacket(PacketBase* pPkt)
{
	if(!pPkt)
		return false;

	if( m_PacketCnt >= MaxCnt)
		return false;

	m_pPacket[m_PacketCnt] = (PacketBase*)PacketFactory::Instance().New( pPkt->GetPacketID() );

	char buffer[PACKET_MAX_SIZE];
	char* pRet = pPkt->WritePacket( buffer );

	int32 nPktSize = (int32)(pRet - buffer);
	if( m_ContentSize + nPktSize >= MaxContentSize )
	{
		FACTORY_DEL_PACKET(m_pPacket[m_PacketCnt]);
		return false;
	}

	m_pPacket[m_PacketCnt]->ReadPacket(buffer);

	m_PacketCnt++;
	m_ContentSize += nPktSize;

	return true;
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

const char* PacketBase::GetClassName()
{
	if( GetClass() && GetClass()->ClassName() )
		return GetClass()->ClassName();

	return "NULL";
}

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
	char* tmp = pBuffer;

	*((int32*)tmp) = m_PacketID;
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
