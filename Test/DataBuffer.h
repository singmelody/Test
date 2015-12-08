#pragma once

#include <assert.h>

class DataBufferBase
{
public:
	DataBufferBase(){}
	virtual ~DataBufferBase(){}
	virtual char* GetBuffer() { return NULL; }
};

template < int32 nBufferSize>
class DataBuffer : public DataBufferBase
{
public:
	DataBuffer()
	{
		memset( m_buffer, 0, bufferSize);
	}

	~DataBuffer(){}

	char* GetBuffer() { return m_buffer; }

protected:
	char m_buffer[nBufferSize];
};

class DataBufferEx : public DataBufferBase
{
public:
	DataBufferEx()
	{
		m_pBuffer = NULL;
	}

	DataBufferEx(int32 nSize)
	{
		m_pBuffer = new char[nSize];
		memset( m_pBuffer, 0, nSize);
	}

	~DataBufferEx()
	{
		SAFE_DELETE(m_pBuffer);
	}

	char* GetBuffer() { return m_pBuffer; }

	void InitBuffer(int32 nSize)
	{
		assert(!m_pBuffer);

		m_pBuffer = new char[nSize];
		memset( m_pBuffer, 0, nSize);
	}

protected:
	char* m_pBuffer;
};

#define MakeDataBufferEx(bufferSize) \
	new DataBufferEx(bufferSize);
