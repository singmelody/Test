#include "StdAfx.h"
#include "AsynContextManager.h"


AsynContextManager::AsynContextManager(void)
	: map( &AsynContext::GetContextID )
{
}


AsynContextManager::~AsynContextManager(void)
{

}

bool AsynContextManager::InitContext(AsynContext* pContext)
{
	int32 nKey = m_vMap.gen_key();
	assert( nKey != InvalidID );
	return nKey;
}

bool AsynContextManager::InsertContext(AsynContext* pContext)
{
	assert( pContext->nContextID != InvalidID);
	return m_vMap.insert(pContext);
}

AsynContext* AsynContextManager::GetContext(int32 nContextID)
{
	auto itr = m_vMap.find(nContextID);
	if( itr == m_vMap.end())
		return NULL;

	return *itr;
}

void AsynContextManager::RemoveContext(int32 nContextID)
{
	m_vMap.erase(nContextID);
}
