#pragma once
#include <assert.h>


#define ENABLE_VAR_INT_T_CLASS	1

#define	ENABLE_VAR_INT


namespace DS
{
	inline uint32 zigzag(int32 n) { return ( n << 1) ^ (n >> 31); }
	inline uint64 zigzag(int64 n) { return ( n << 1) ^ (n >> 64); }

	inline int32 zagzig(uint32 n) { return ( n >> 1) ^ -(int32(n & 1)); }
	inline int64 zagzig(uint64 n) { return ( n >> 1) ^ -(int64(n & 1)); }

	struct Vec3
	{
		f32 x,y,z;
	};

	inline bool SerializeVarInt(uint32& value, void* buffer, uint32& offset, bool bToBuff, size_t bufferSize = 65535);
	inline bool SerializeVarInt(int32& value, void* buffer, uint32& offset, bool bToBuff, size_t bufferSize = 65535)
	{
		bool bRet = false;
		uint32 vv;
		if(bToBuff)
		{
			vv = zigzag(value);
			bRet = SerializeVarInt( vv, buffer, offset, bToBuff, bufferSize);
		}
		else
		{
			bRet = SerializeVarInt( vv, buffer, offset, bToBuff, bufferSize);
		}

		return true;
	}

	inline bool SerializeVarInt(uint64& value, void* buffer, uint32& offset, bool bToBuff, size_t bufferSize = 65535);
	inline bool SerializeVarInt(int64& value, void* buffer, uint32& offset, bool bToBuff, size_t bufferSize = 65535)
	{
		bool bRet = false;
		uint64 vv;
		if(bToBuff)
		{
			vv = zigzag(value);
			bRet = SerializeVarInt( vv, buffer, offset, bToBuff, bufferSize);
		}
		else
		{
			bRet = SerializeVarInt( vv, buffer, offset, bToBuff, bufferSize);
		}

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

		inline bool IsIStream() const { return m_bIsStream; }
		inline bool IsOStream() const { return !IsIStream(); }

		template <typename T>
		inline MemoryIOStreamBase& SerializeT(T& data)
		{
			return SerializeBlob( &data, sizeof(data));
		}

		inline MemoryIOStreamBase& SerializeF32AsInt16(float& data, f32 fScale)
		{
			assert( fabs(fScale) > 1e-6 );

			if(IsOStream())
			{
				int16 tmp = int16( data * fScale );
				SerializeT(tmp);
			}
			else
			{
				int16 tmp = 0;
				SerializeT(tmp);
				data = tmp / fScale;
			}

			return *this;
		}

		inline MemoryIOStreamBase& SerializeF32AsInt16(Vec3& data, f32 fScale)
		{
			return SerializeF32AsInt16(data.x, fScale).SerializeF32AsInt16( data.y, fScale).SerializeF32AsInt16( data.z, fScale);
		}

		inline MemoryIOStreamBase& SerializeBlob(void* pData, size_t dataLen)
		{
			assert(pData);
			char* pCurr = reserve(dataLen);
			if(pCurr && pData)
			{
				if(IsOStream())
				{
					memcpy( pCurr, pData, dataLen);
				}
				else
				{
					memcpy( pData, pCurr, dataLen);
				}
			}
			return *this;
		}

		inline MemoryIOStreamBase& SerializeVarInt(uint32& data)
		{
			uint32 nOffset = 0;
			if(!DS::SerializeVarInt( data, GetBuffer(), nOffset, IsOStream(), GetBufferLen()))
				SetError();
			else
				reserve(nOffset);

			return *this;
		}

		inline MemoryIOStreamBase& SerializeVarInt64(uint64& data)
		{
			uint32 nOffset = 0;
			if(!DS:SerializeVarInt( data, GetBuffer(), nOffset, IsOStream(), GetBufferLen()))
				SetError();
			else
				reserve(nOffset);

			return *this;
		}
	private:
		bool m_bIsStream;
	};

	template <>
	inline MemoryIOStreamBase& MemoryIOStreamBase::SerializeT<int32>(int32 data)
	{
		if(IsOStream())
		{
			uint32 tmp = DS::zigzag(data);
			SerializeVarInt(tmp);
		}
		else
		{
			uint32 tmp = 0;
			SerializeVarInt(tmp);
			data = DS::zagzig(tmp);
		}
	}

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