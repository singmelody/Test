#include "StdAfx.h"
#include <sstream>
#include "ParamPool2Sql_DBA.h"
#include "DBASqlHelper.h"
#include "ParamPool.h"
#include "ParamDef.h"

ParamPoolExInfo_DBA_StateBase::ParamPoolExInfo_DBA_StateBase(void)
{
}


ParamPoolExInfo_DBA_StateBase::~ParamPoolExInfo_DBA_StateBase(void)
{
}

IParamPool2SqlProcessor::EDataState ParamPool2SqlProcessor_DBA_Avatar::GetDataState(ParamPool& pool) const
{
	assert(pool.SHMGetExtraMemory());
	ParamPoolExInfo_DBA_Avatar& exInfo = *(ParamPoolExInfo_DBA_Avatar*)pool.SHMGetExtraMemory();
	return exInfo.GetState();
}

// ParamPool2SqlProcessor_DBA_Avatar
bool ParamPool2SqlProcessor_DBA_Avatar::GetSqlString_Insert(ParamPool& pool, std::string& strSql) const
{
	if(!pool.GetParamDef())
		return false;

	std::stringstream ss;
	ss<<"call p_insert_"<<pool.GetParamDef()->Name()<<"("<<
		pool.GetParamTypeID()<<","<<DBASqlHelper::GetProcString( pool.GetParamDef(), &pool)<<")";

	return true;
}

bool ParamPool2SqlProcessor_DBA_Avatar::GetSqlString_Update(ParamPool& pool, std::string& strSql) const
{
	strSql.clear();

	std::stringstream ss;
	ParamDef* pParamDef = pool.GetParamDef();
	bool bRet = DBASqlHelper::GetUpdateSqlString( pParamDef, &pool, strSql);
	if(!bRet)
		return false;

	int64 nAvatarID = PARAM_GET_VALUE( (&pool), avatardid, int64(0));
	ss<<strSql<<" where avatardid = "<< nAvatarID<<";";
	strSql = ss.str();

	return true;
}

bool ParamPool2SqlProcessor_DBA_Avatar::GetSqlString_Delete(ParamPool& pool, std::string& strSql) const
{
	return false;
}


IParamPool2SqlProcessor::EDataState ParamPool2Sqlprocessor_DBA_Avatar_CommonData::GetDataState(ParamPool& pool) const
{
	assert( pool.SHMGetExtraMemory() );
	ParamPoolExInfo_DBA_CommonData& exInfo = *(ParamPoolExInfo_DBA_CommonData*)pool.SHMGetExtraMemory();
	return exInfo.GetState();
}

// ParamPool2Sqlprocessor_DBA_Avatar_CommonData
bool ParamPool2Sqlprocessor_DBA_Avatar_CommonData::GetSqlString_Insert(ParamPool& pool, std::string& strSql) const
{
	assert(pool.SHMGetExtraMemory());

	ParamPoolExInfo_DBA_CommonData& exInfo = *(ParamPoolExInfo_DBA_CommonData*)pool.SHMGetExtraMemory();
	std::stringstream ss;
	ss<<"call p_insert_"<<pool.GetParamDef()->Name()<<
		"("<<exInfo.m_avatarDID<<","<<PARAM_DATA_ID(exInfo.m_typeID)<<","<<exInfo.m_index<<","<<DBASqlHelper::GetProcString(pool.GetParamDef(), &pool)<<");";

	strSql = ss.str();
	return true;
}

bool ParamPool2Sqlprocessor_DBA_Avatar_CommonData::GetSqlString_Update(ParamPool& pool, std::string& strSql) const
{
	strSql.clear();

	std::string ss;
	ParamDef* pParamDef = pool.GetParamDef();
	bool bRet = DBASqlHelper::GetUpdateSqlString( pParamDef, &pool, strSql);
	if(!bRet)
		return false;

	int64 nAvatarDID = PARAM_GET_VALUE( (&pool), avatardid, int64(0));

	return true;
}

bool ParamPool2Sqlprocessor_DBA_Avatar_CommonData::GetSqlString_Delete(ParamPool& pool, std::string& strSql) const
{
// 	int64 gH = PARAM_GET_VALUE( (&pool), guildhigh, int64(0));
// 	int64 gL = PARAM_GET_VALUE( (&pool), guildlow, int64(0));
	return true;
}
