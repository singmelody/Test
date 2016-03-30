#pragma once

#define ENABLE_VAR_INT_T_CLASS	1

#define	ENABLE_VAR_INT


namespace DS
{
	class Vec3
	{
		f32 x,y,z;
	};

	inline bool SerializeVarInt()
	{

	}

	template <>
	inline void Serialize(int32& value, void* buff, uint32& offset, bool bToBuff)
	{
		Serialize( value, buff, offset, bToBuff);
	}

	template <>
	inline void Serialize(uint32& value, void* buff, uint32& offset, bool bToBuff)
	{
		Serialize( value, buff, offset, bToBuff);
	}

	template <>
	inline void Serialize(Vec3& value, void* buffer, uint32& offset, bool bToBuff)
	{
		Serialize( value.x, buffer, offset, bToBuff);
		Serialize( value.y, buffer, offset, bToBuff);		
		Serialize( value.z, buffer, offset, bToBuff);
	}
}