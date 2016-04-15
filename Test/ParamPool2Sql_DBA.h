#pragma once

#include "ParamPool2SqlProcessor.h"

#pragma pack( push, 1)

class ParamPoolExInfo_DBA_StateBase
{
public:
	ParamPoolExInfo_DBA_StateBase(void);
	~ParamPoolExInfo_DBA_StateBase(void);

	inline void SetLoaded() { m_dataState = IParamPool2SqlProcessor::eParamDS_Loaded; }
	inline bool IsLoaded() const { return m_dataState == IParamPool2SqlProcessor::eParamDS_Loaded; }
	
	inline void SetInsert() { m_dataState = IParamPool2SqlProcessor::eParamDS_Insert; }
	inline bool IsInsert() const { return m_dataState == IParamPool2SqlProcessor::eParamDS_Insert; }

	inline void SetUpdateSafe()
	{
		if(!IsInsert())
			m_dataState = IParamPool2SqlProcessor::eParamDS_Update;
	}


	inline void SetUpdateForce()
	{
		m_dataState = IParamPool2SqlProcessor::eParamDS_Update;
	}

	inline bool IsUpdate() const { return m_dataState == IParamPool2SqlProcessor::eParamDS_Update; }

	inline void SetUserFlag(uint8 v) { m_userFlag = v; }
	inline uint8 GetUserFlag() const { return m_userFlag; }
protected:
	uint8 m_dataState:4;
	uint8 m_userFlag:4;
};

struct ParamPoolExInfo_DBA_Avatar : public ParamPoolExInfo_DBA_StateBase
{

};

struct ParamPoolExInfo_DBA_CommonData : public ParamPoolExInfo_DBA_StateBase
{
	inline void SetIndexChanged(bool bChg) { SetUserFlag( bChg ? 1 : 0); }
	inline bool IsIndexChanged() const { return GetUserFlag() == 1; }

	uint8	m_dataState;
	int64	m_avatarDID;
	int64	m_typeID;
	int32	m_index;
};

#pragma pack(pop)

class ParamPool2SqlProcessor_DBA_Avatar : public IParamPool2SqlProcessor
{
	virtual uint32 GetExtraSize() const { return sizeof(*this); }
	virtual IParamPool2SqlProcessor::EDataState GetDataState(ParamPool& pool) const;

	virtual bool GetSqlString_Insert( ParamPool& pool, std::string& strSql) const;
	virtual bool GetSqlString_Update( ParamPool& pool, std::string& strSql) const;
	virtual bool GetSqlString_Delete( ParamPool& pool, std::string& strSql) const;
};

class ParamPool2Sqlprocessor_DBA_CommonData : public IParamPool2SqlProcessor
{
	virtual uint32 GetExtraSize() const { return sizeof(*this); }
	virtual IParamPool2SqlProcessor::EDataState GetDataState(ParamPool& pool) const;

	virtual bool GetSqlString_Insert( ParamPool& pool, std::string& strSql) const;
	virtual bool GetSqlString_Update( ParamPool& pool, std::string& strSql) const;
	virtual bool GetSqlString_Delete( ParamPool& pool, std::string& strSql) const;
};