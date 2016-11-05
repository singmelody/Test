#include "StdAfx.h"
#include "BaseTestUnit.h"


void SetColor(unsigned short attr)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, attr);
}


BaseTestUnit::BaseTestUnit(void)
{
}


BaseTestUnit::~BaseTestUnit(void)
{
}