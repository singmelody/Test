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
}