#pragma once
#include <stdlib.h>

class MyString
{
public:
	MyString(void);
	~MyString(void);

	template <class T>
	static T ToValue(const char* valStr);
};

template<>
inline int8 MyString::ToValue(const char* valStr)
{
	int8 nVal;
	nVal = (int8)MyString<int32>::ToValue(valStr);
	return nVal;
}

template <>
inline uint8 MyString::ToValue(const char* valStr)
{
	uint8 nVal;
	nVal = (uint8)MyString<uint32>::ToValue(valStr);
	return nVal;
}

template <>
inline int16 MyString::ToValue(const char* valStr)
{
	int16 nVal;
	nVal = (int16)MyString<int32>::ToValue(valStr);
	return nVal;
}

template <>
inline uint16 MyString::ToValue(const char* valStr)
{
	uint16 nVal;
	nVal = (uint16)MyString<uint32>::ToValue(valStr);
	return nVal;
}

template <>
inline int32 MyString::ToValue(const char* valStr)
{
	int32 nVal = atoi(valStr);
	return nVal;
}

template <>
inline uint32 MyString::ToValue(const char* valStr)
{
	uint32 nVal;
	sscanf( valStr, "%u", &nVal);
	return nVal;
}

template <>
inline int64 MyString::ToValue(const char* valStr)
{
	int64 value = 0;
	scanf( value, "%I64i", &value);
}

template <>
inline uint64 MyString::ToValue(const char* valStr)
{
	uint64 nVal;
	sscanf( valStr, "%I64u", &nVal);
	return nVal;
}