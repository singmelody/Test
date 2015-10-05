#include "stdafx.h"
#include "UtilID.h"
#include <assert.h>
#include "DBInterface.h"
#include "LoadTemplateManager.h"

// unit test
#include "TestODBCManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int tt = UtilID::CreateFromString("wangyanbin");
	assert(tt == 65398277);

	FACTORY_NEWOBJ(GDBRow);
	FACTORY_NEWOBJ(GDBRow);

	LoadTemplateManager templateMgr;

	templateMgr.AddTemplate("TestODBCManager", TestODBCManager::Instance());

	printf("I am here waiting for u\n");

	return 0;
}

