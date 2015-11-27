#include "stdafx.h"
#include "UtilID.h"
#include <assert.h>
#include "DBInterface.h"
#include "LoadTemplateManager.h"
#include "CommonConfig.h"
#include "MyLog.h"
#include "Log.h"
#include "LogThread.h"
// unit test
#include "TestODBCManager.h"

#include "RSAipher.h"

#include "MyVector.h"
#include "MyMath.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Vector2 t(3,4);
	t.Normalize();

	assert(t.x > (0.6000 - 0.000001f) && t.x < (0.6000 + 0.000001f));
	assert(t.y > (0.8000 - 0.000001f) && t.y < (0.8000 + 0.000001f));

	ShapeLine2D line0,line1;

	line0.p0.x = 1.0f;
	line0.p0.y = 2.0f;
	line0.p1.x = 3.0f;
	line0.p1.y = 3.0f;

	line1.p0.x = 1.0f;
	line1.p0.y = 0.0f;
	line1.p1.x = 3.0f;
	line1.p1.y = 2.0f;

	bool b = Math::LineCross( line0, line1);
	assert(b == false);

	return 0;

	int32 nK = 0x12345678;
	g_pLog = new Log();
	MyLog::Create( "log.txt");
	LogThread::Instance().Start();

	CommonConfig::DBType	= eDBType_DBC;
	CommonConfig::DBCName	= "testdb";
	CommonConfig::DBCUser	= "root";
	CommonConfig::DBCPwd= "123456";
	
	unsigned int tt = UtilID::CreateFromString("wangyanbin");
	assert(tt == 65398277);
	
	FACTORY_NEWOBJ(DBRow);
	FACTORY_NEWOBJ(DBRow);
	
	LoadTemplateManager templateMgr;
	
	templateMgr.AddTemplate("TestODBCManager", TestODBCManager::Instance());
	templateMgr.Load("NodeServer");

	MyLog::message("I am here waiting for u");


	FILE* pFile = fopen( "my.public", "r");
	RSAipher* pPtr = RSAipher::Create( pFile, true);
	RSAipher::Destroy( pPtr );
	pPtr = NULL;
	fclose(pFile);	

	pFile = fopen( "my.private", "r");
	pPtr = RSAipher::Create( pFile, false);
	RSAipher::Destroy( pPtr );
	pPtr = NULL;
	fclose(pFile);
	return 0;
}

