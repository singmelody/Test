#pragma once

#include "Singleton.h"
#include "LoadTemplateManager.h"


class MyLoadTemplateManager : public LoadTemplateManager, public Singleton<MyLoadTemplateManager>
{
public:
	MyLoadTemplateManager(void);
	~MyLoadTemplateManager(void);

	virtual MyFile* CreateLoadFile();
	virtual void ReleaseLoadFile(MyFile* pFile);
};

