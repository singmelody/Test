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
