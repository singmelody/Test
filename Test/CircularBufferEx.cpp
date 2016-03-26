#include "StdAfx.h"
#include "CircularBufferEx.h"
#include <assert.h>

CircularBufferEx::CircularBufferEx(void)
{
	m_buffer = m_dataStart = m_dataEnd = m_bufferEnd = NULL;
}


CircularBufferEx::~CircularBufferEx(void)
{
	SAFE_DELETE_ARRAY(m_buffer);
	m_dataStart = m_dataEnd = m_bufferEnd = NULL;
}


void CircularBufferEx::Allocate(uint32 nSize)
{
	assert(m_buffer == NULL);

	m_buffer = new uint8[nSize];
	m_bufferEnd = m_buffer + nSize;
	m_dataStart = m_dataEnd = m_buffer;
}


bool CircularBufferEx::Read(void* destionation, uint32 bytes)
{
	assert(m_buffer);

	if(GetSize() < bytes)
		return false;

	memcpy((char*)destionation, m_dataStart, bytes);
	m_dataStart += bytes;
	return true;
}

bool CircularBufferEx::Write(const void* data, uint32 bytes)
{
	assert(m_buffer);

	void* pBuffer = ReserveForWrite( bytes, true);
	if(!pBuffer)
		return false;

	memcpy( pBuffer, data, bytes);
	return true;
}

void CircularBufferEx::Remove(uint32 nLen)
{
	if(GetSize() >= nLen)
		m_dataStart += nLen;
}

void* CircularBufferEx::ReserveForWrite( uint32 bytes, bool bMoveHeadIfNeed)
{
	if(GetSpaceAfterData() < bytes)
	{
		if(bMoveHeadIfNeed)
		{
			MoveData2Head();
			if(GetSpaceAfterData() < bytes)
				return 0;
		}
		else
			return 0;
	}

	void* pRet = m_dataEnd;
	m_dataEnd += bytes;
	return pRet;
}

void CircularBufferEx::MoveData2Head()
{
	if( m_buffer == m_dataStart )
		return;

	const uint32 dataSize = GetSize();
	if(dataSize > 0)
		memmove( m_buffer, m_dataStart, dataSize);

	m_dataStart = m_buffer;
	m_dataEnd = m_dataStart + dataSize;
}