// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UtilID.h"
#include <assert.h>
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int tt = UtilID::CreateFromString("wangyanbin");
	assert(tt == 65398277);


	return 0;
}

