#include "StdAfx.h"
#include "SceneProcessCont.h"


SceneProcessCont::SceneProcessCont(void)
{
}


SceneProcessCont::~SceneProcessCont(void)
{
}

SceneProcess* SceneProcessCont::GetProcess(uint16 nSceneSID)
{
	SceneProcessMap::iterator itr = m_mapProcess.find(nSceneSID);
	if( itr == m_mapProcess.end() )
		return NULL;

	return itr->second;
}


