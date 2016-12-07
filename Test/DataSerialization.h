#pragma once
#include <assert.h>


#define ENABLE_VAR_INT_T_CLASS	1

#define	ENABLE_VAR_INT


namespace DS
{
	struct Vec3
	{
		f32 x,y,z;
	};

	inline bool SerializeVarInt(uint32& value, void* buff, uint32& offset, bool bToBuff, size_t bufferSize = 65535);
	inline bool SerializeVarInt(int32& value, void* buff, uint32& offset, bool bToBuff, size_t bufferSize = 65535)
	{
		return true;
	}

	inline bool SerializeVarInt(uint64& value, void* buff, uint32& offset, bool bToBuff, size_t bufferSize = 65535);
	inline bool SerializeVarInt(int64& value, void* buff, uint32& offset, bool bToBuff, size_t bufferSize = 65535)
	{
		return true;
	}

	template< class T>
	inline void Serialize( T& value, void* buff, uint32& offset, bool b2Buff)
	{
		assert(buff);

		T* pData = (T*)((char*)buff + offset);
		if(b2Buff)
		{
			*pData = value;
		}
		else
		{
			value = *pData;
		}

		offset += sizeof(T);
	}


	template <>
	inline void Serialize(int32& value, void* buff, uint32& offset, bool bToBuff)
	{
		SerializeVarInt( value, buff, offset, bToBuff);
	}

	template <>
	inline void Serialize(uint32& value, void* buff, uint32& offset, bool bToBuff)
	{
		SerializeVarInt( value, buff, offset, bToBuff);
	}

	template <>
	inline void Serialize(Vec3& value, void* buffer, uint32& offset, bool bToBuff)
	{
		Serialize( value.x, buffer, offset, bToBuff);
		Serialize( value.y, buffer, offset, bToBuff);		
		Serialize( value.z, buffer, offset, bToBuff);
	}

	class IOStreamBase
	{
	public:
		IOStreamBase(char* pStart, char* pEnd)
			:m_pBuffer(pStart)
			,m_pBufferBegin(pStart)
			,m_pBufferEnd(pEnd)
			,m_bError(false)
		{
			assert( m_pBuffer && m_pBufferEnd && m_pBuffer <= m_pBufferEnd);
		}

	private:
		char* m_pBuffer;
		char* const m_pBufferBegin;
		char* const m_pBufferEnd;
		bool m_bError;
	};

	class MemoryIOStreamBase : public IOStreamBase
	{
	public:
		MemoryIOStreamBase(bool bIsStream, char* bufferStart, char* bufferEnd)
			: IOStreamBase( bufferStart, bufferEnd), m_bIsStream(bIsStream)
		{

		}

	private:
		bool m_bIsStream;
	};

	class MemoryOStream : public MemoryIOStreamBase
	{
	public:
		MemoryOStream(char* pBufferStart, char* pBufferEnd): MemoryIOStreamBase( false, pBufferStart, pBufferEnd){}
		MemoryOStream(char* pBufferStart, size_t nBufferLen): MemoryIOStreamBase( false, pBufferStart, pBufferStart + nBufferLen){}

	private:
		MemoryOStream(const MemoryOStream& );
		MemoryOStream& operator= (const MemoryOStream&);
	};

	class MemoryIStream : public MemoryIOStreamBase
	{
	public:
		MemoryIStream(char* pBufferStart, char* pBufferEnd): MemoryIOStreamBase( false, pBufferStart, pBufferEnd){}
		MemoryIStream(char* pBufferStart, size_t nBufferLen): MemoryIOStreamBase( false, pBufferStart, pBufferStart + nBufferLen){}
	private:
		MemoryIStream(const MemoryIStream& );
		MemoryIStream& operator= (const MemoryIStream&);
	};
}