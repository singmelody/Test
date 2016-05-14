#include "StdAfx.h"
#include "MyLoadTemplateManager.h"
#include "MyFile.h"

MyLoadTemplateManager::MyLoadTemplateManager(void)
{
}


MyLoadTemplateManager::~MyLoadTemplateManager(void)
{
}

MyFile* MyLoadTemplateManager::CreateLoadFile()
{
	return new SimpleFile();
}

void MyLoadTemplateManager::ReleaseLoadFile(MyFile* pFile)
{
	SAFE_DELETE(pFile);
}

