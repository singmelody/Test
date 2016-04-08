#include "StdAfx.h"
#include "ParamPool2SqlProcessor.h"
#include "MyLog.h"
#include "ParamDefManager.h"

ParamPool2SqlProcessorManagerBase::ParamPool2SqlProcessorManagerBase(void)
{

}


ParamPool2SqlProcessorManagerBase::~ParamPool2SqlProcessorManagerBase(void)
{
	for (std::map<int32, IParamPool2SqlProcessor*>::iterator itr = m_mapParamIdx2Processor.begin(); 
		itr != m_mapParamIdx2Processor.end(); ++itr)
	{
		SAFE_DELETE(itr->second);
	}
}


void ParamPool2SqlProcessorManagerBase::AddParam2SqlProcessor(int32 nParamIdx, IParamPool2SqlProcessor* pProcessor)
{
	if(!m_mapParamIdx2Processor.insert(std::make_pair( nParamIdx, pProcessor)).second)
	{
		MyLog::error("ParamPool2SqlProcessorManagerBase::AddSHM2SqlProcessor Failed idex = [%d]", nParamIdx);
	}
}

void ParamPool2SqlProcessorManagerBase::RegisterProcessor()
{
	const ParamMap& paramMap = ParamDefManager::Instance().GetDefMap();

	for (ParamMap::const_iterator itr = paramMap.begin(); itr != paramMap.end(); ++itr)
	{
		ParamDef* pDef = itr->second;
		assert(pDef);

		IParamPool2SqlProcessor* pIParamPool2SqlProcessor = GetParam2SqlProcessor(pDef->Index());
		if(!pIParamPool2SqlProcessor)
			continue;

		pDef->SetIParamPool2SqlProcessor(pIParamPool2SqlProcessor);
	}
}

bool IParamPool2SqlProcessor::GetSqlString(ParamPool& pool, std::string& strSql) const
{
	EDataState state = GetDataState(pool);

	switch(state)
	{
	case eParamDS_Insert: return GetSqlString_Insert( pool, strSql);
	case eParamDS_Update: return GetSqlString_Update( pool, strSql);
	case eParamDS_Loaded: return false;
	}

	MyLog::error("IParamPool2SqlProcessor::GetSqlString bad data state [%d]", int32(state));

	return true;
}