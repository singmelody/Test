#pragma once
#include <stdlib.h>


typedef unsigned long long uint64;
typedef long long int64;
typedef char Char;
typedef float f32;
typedef double f64;

typedef unsigned char uint8;
typedef signed char	int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;

class MyString
{
public:
	MyString(void);
	~MyString(void);

	template <class T>
	static T ToValue(const char* valStr);
};


template<>
inline int32 MyString::ToValue<int32>(const char* valStr)
{
	int32 nVal = atoi(valStr);
	return nVal;
}

template<>
inline uint32 MyString::ToValue<uint32>(const char* valStr)
{
	uint32 nVal;
	sscanf( valStr, "%u", &nVal);
	return nVal;
}

template<>
inline int8 MyString::ToValue<int8>(const char* valStr)
{
	int8 nVal;
	nVal = (int8)MyString::ToValue<int32>(valStr);
	return nVal;
}

template<>
inline uint8 MyString::ToValue<uint8>(const char* valStr)
{
	uint8 nVal;
	nVal = (uint8)MyString::ToValue<uint32>(valStr);
	return nVal;
}

template<>
inline int16 MyString::ToValue<int16>(const char* valStr)
{
	int16 nVal;
	nVal = (int16)MyString::ToValue<int32>(valStr);
	return nVal;
}

template<>
inline uint16 MyString::ToValue<uint16>(const char* valStr)
{
	uint16 nVal;
	nVal = (uint16)MyString::ToValue<uint32>(valStr);
	return nVal;
}

template<>
inline int64 MyString::ToValue<int64>(const char* valStr)
{
	int64 nValue = 0;
	scanf( valStr, "%I64i", &nValue);
	return nValue;
}

template<>
inline uint64 MyString::ToValue<uint64>(const char* valStr)
{
	uint64 nVal;
	sscanf( valStr, "%I64u", &nVal);
	return nVal;
}

template<>
inline f32 MyString::ToValue<f32>(const char* valStr)
{
	f32 nVal;
	nVal = (f32)atof(valStr);
	return nVal;
}

template<>
inline double MyString::ToValue<double>(const char* valStr)
{
	double nVal;
	nVal = (double)atof(valStr);
	return nVal;
}
