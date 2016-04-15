#pragma once

#include <string>
#include <map>
#include "BaseType.h"

class ParamPool;

struct IParamPool2SqlProcessor
{
	enum EDataState
	{
		eParamDS_Unkown = 0,
		eParamDS_Loaded,
		eParamDS_Insert,
		eParamDS_Update
	};

	virtual ~IParamPool2SqlProcessor(){}

	virtual bool UseSHM() const { return false; }
	virtual uint32 GetExtraSize() const { return 0; }

	bool GetSqlString( ParamPool& pool, std::string& strSql) const;

	virtual EDataState GetDataState( ParamPool& pool) const { return eParamDS_Unkown; }
	virtual bool GetSqlString_Insert( ParamPool& pool, std::string& strSql) const { return false; }
	virtual bool GetSqlString_Delte( ParamPool& pool, std::string& strSql) const { return false; }
	virtual bool GetSqlString_Update( ParamPool& pool, std::string& strSql) const { return false; }
};


class ParamPool2SqlProcessorManagerBase
{
public:
	ParamPool2SqlProcessorManagerBase(void);
	virtual ~ParamPool2SqlProcessorManagerBase(void);

	IParamPool2SqlProcessor* GetParam2SqlProcessor(int32 nParamIdx) const;

protected:
	void AddParam2SqlProcessor( int32 nParamIdx, IParamPool2SqlProcessor* pProcessor);
	void RegisterProcessor();

	typedef std::map< int32, IParamPool2SqlProcessor*> ParamProcessorMap;
	ParamProcessorMap m_mapParamIdx2Processor;
};

