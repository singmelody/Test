#pragma once

#include <map>
#include "BaseType.h"

class AsynContext
{
public:
	AsynContext();
	virtual ~AsynContext() {}

	int32	nContextID;
	uint16	nContextType;

	int32 GetContextID()
	{
		return nContextID;
	}
};

class AsynContextManager
{
public:
	AsynContextManager(void);
	virtual ~AsynContextManager(void);

	bool InitContext(AsynContext* pContext);

	int32 GenerateKey();

	bool InsertContext( AsynContext* pContext);
	AsynContext* GetContext(int32 nContextID);
	void RemoveContext( int32 nContextID );
protected:
	typedef std::map< int32, AsynContext*> ContextMap;
	ContextMap m_vMap;
};

