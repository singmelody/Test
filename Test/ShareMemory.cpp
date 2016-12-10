#include "StdAfx.h"
#include "ShareMemory.h"
#include "MyLog.h"
#include "UtilID.h"
#include <assert.h>
#include <Windows.h>

ShareMemory::ShareMemory(void)
{
	Reset();
}


ShareMemory::~ShareMemory(void)
{
	Close();
}


bool ShareMemory::Open( const char* szPath, SM_KEY nKey, uint32 nSize, bool& bCreated)
{
	assert( m_SMMemPtr == 0 );

	if(nSize <= 0)
		return false;

	m_SMKey = UtilID::CreateFromString(szPath) + nKey;
	MyLog::message("OpenSHM path=[%s].[%d] KEY=[0x%x] size=[%u]", szPath, nKey, m_SMKey, nSize);

	{
		char buf[128];
		sprintf( buf, "%u", nKey);
		m_SMName = std::string(szPath) + buf;
	}

	MyLog::message("OpenSHM path=[%s].key=[%d].size=[%u]", szPath, nKey, nSize);

	m_SMSize = nSize;

	return true;
}

void ShareMemory::Close()
{
	Detach();

	CloseShareMemory();

	Reset();
}

void ShareMemory::Detach()
{
	if( m_SMMemPtr )
	{
		UnmapViewOfFile(m_SMMemPtr);
	}
}

void ShareMemory::Reset()
{
	m_SMName.clear();

	m_SMSize = 0;
	m_SMHandle = INVALID_SMHANDLE;
	m_SMMemPtr = NULL;
}

void* ShareMemory::GetShareMemoryPtr() const
{
	return m_SMMemPtr;
}

void ShareMemory::CloseShareMemory()
{
	if( m_SMHandle != INVALID_SMHANDLE )
		CloseHandle(m_SMHandle);
}

void ShareMemory::SHMRM(const char* szPath, SM_KEY nKey)
{
	return;
}

