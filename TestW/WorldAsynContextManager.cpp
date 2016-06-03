#include "StdAfx.h"
#include "WorldAsynContextManager.h"


WorldAsynContextManager::WorldAsynContextManager(void)
{
}


WorldAsynContextManager::~WorldAsynContextManager(void)
{
}

AsynContext* WorldAsynContextManager::NewContext(char* pClassName)
{
	AsynContext* pContext = (AsynContext*)FactoryManager::Instance().New( pClassName );
	if(!pContext)
		return NULL;

	InitContext( pContext );

	if( !InsertContext( pContext ))
	{
		FACTORY_DELOBJ( pContext );
		return NULL;
	}

	return pContext;
}

void WorldAsynContextManager::FreeContext(AsynContext* pContext)
{
	RemoveContext( pContext->nContextID );
	FACTORY_DELOBJ(pContext);
}

