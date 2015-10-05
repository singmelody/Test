#include "StdAfx.h"
#include "LoadTemplate.h"
#include "DBInterface.h"
#include "LoadTemplateManager.h"

LoadTemplate::LoadTemplate(void)
{
}


LoadTemplate::~LoadTemplate(void)
{
}

LoadTemplate* LoadTemplate::GetTemplate(const char* pStr)
{
	return NULL;
}

MyFile* LoadTemplate::CreateLoadFile()
{
	if(!m_pLoadMgr)
		return NULL;

	return m_pLoadMgr->CreateLoadFile();
}

void LoadTemplate::ReleaseLoadFile(MyFile*& pFile)
{
	m_pLoadMgr->ReleaseLoadFile(pFile);
}
