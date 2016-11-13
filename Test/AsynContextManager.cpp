#include "StdAfx.h"
#include "AsynContextManager.h"


AsynContextManager::AsynContextManager(void)
	: m_IDPool("AsynContext", 0xffff)
{
}


AsynContextManager::~AsynContextManager(void)
{

}

bool AsynContextManager::InitContext(AsynContext* pContext)
{
	pContext->nContextID = 0;
	m_IDPool.id_alloc( pContext->nContextID );

	if( pContext->nContextID == 0)
		return false;

	return true;
}

bool AsynContextManager::InsertContext(AsynContext* pContext)
{
	return m_vMap.Insert( pContext->nContextID,  pContext);
}

AsynContext* AsynContextManager::GetContext(int32 nContextID)
{
	if( m_vMap.Find(nContextID) )
		return m_vMap.Get();

	return NULL;
}

void AsynContextManager::RemoveContext(int32 nContextID)
{
	m_vMap.Remove(nContextID);
	m_IDPool.free_id(nContextID);
}
