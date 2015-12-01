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

template <>
inline int32 MyString::ToValue(const char* valStr)
{
	return atoi(valStr);
}

template <>
inline uint32 MyString::ToValue(const char* valStr)
{
	uint32 nVal;
	sscanf( valStr, "%u", &nVal);
	return nVal;
}