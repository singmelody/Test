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


MyClass::MyClass()
{
	_parent = NULL;

	nClassID = -1;
	nClassSize = 0;
	nClassSerSize = 0;
}

MyClass::~MyClass()
{
	for (MemberMap::iterator itr = mMemberMap.begin(); itr != mMemberMap.end(); ++itr)
		SAFE_DELETE(itr->second);
}

void MyClass::AddMember(TypeBase* pType)
{
	if(!pType)
		return;

	MemberMap::iterator itr = mMemberMap.find(pType->Name());
	if( itr != mMemberMap.end() )
		return;

	mMemberMap[pType->Name()] = pType;
	nClassSize += pType->Size();
	if(pType->CheckTypeFlag(eTB_Flag_Sec))
		nClassSerSize += pType->Size();
}

TypeBase* MyClass::GetMember(int32 nIdx)
{
	if( nIdx < 0 || nIdx > (int32)mMemberMap.size())
		return NULL;

	MemberMap::iterator itr = mMemberMap.begin();
	std::advance( itr, nIdx);

	return itr->second;
}

TypeBase* MyClass::GetMember(char* sName)
{
	if(!sName)
		return NULL;

	MemberMap::iterator itr = mMemberMap.find(sName);
	if( itr == mMemberMap.end() )
		return NULL;
	
	return itr->second;
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

	return tmp;
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