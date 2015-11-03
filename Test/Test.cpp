// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UtilID.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int tt = UtilID::CreateFromString("wangyanbin");
	printf("%ud\n", tt);
	return 0;
}

