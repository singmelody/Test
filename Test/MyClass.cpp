#include "StdAfx.h"
#include "MyClass.h"

char* TypeBase::Read(void* pClassObj, char* pBuffer)
{
	if(!pBuffer)
		return NULL;

	memcpy( pPtr, pBuffer, m_size);
	return pBuffer + m_size;
}


char* TypeBase::Read(void* pClassObj, char* pBuffer, bool& dirty)
{
	dirty = true;
	if(!pBuffer)
		return NULL;

	memcpy( pPtr, pBuffer, m_size);
	return pBuffer + m_size;
}

char* TypeBase::Write(void* pClassObj, char* pBuffer)
{
	if(!pBuffer)
		return NULL;

	memcpy( pBuffer, pPtr, m_size);
	return pBuffer + m_size;
}


char* MyClass::Read(void* pClassObj, char* buffer, int32 nFlag /*= eTB_Flag_Sec*/)
{
	if(!buffer)
		return NULL;

	if(!pClassObj)
		return buffer;

	char* tmp = buffer;
	MemberMap::iterator itr = mMemberMap.begin();
	MemberMap::iterator itrEnd = mMemberMap.end();

	while ( itr != itrEnd )
	{
		if(itr->second->CheckTypeFlag(nFlag))
		{
			tmp = itr->second->Read( pClassObj, tmp);
		}

		++itr;
	}
}

char* MyClass::Write(void* pClassObj, char* buffer, int32 nFlag /*= eTB_Flag_Sec*/)
{
	if(!pClassObj || !buffer)
		return buffer;

	char* tmp = buffer;
	MemberMap::iterator itr = mMemberMap.begin();
	MemberMap::iterator itrEnd = mMemberMap.end();

	while( itr != itrEnd )
	{
		if (itr->second->CheckTypeFlag(nFlag))
		{
			tmp = itr->second->Write( pClassObj, tmp);
		}

		++itr;
	}

	return tmp;
}