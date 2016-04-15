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

bool ParamPool2SqlProcessor_DBA_Avatar::GetSqlString_Insert(ParamPool& pool, std::string& strSql) const
{
	if(!pool.GetParamDef())
		return false;

	std::stringstream ss;
	ss<<"call p_insert_"<<pool.GetParamDef()->Name()<<"("<<
		pool.GetParamTypeID()<<","<<DBASqlHelper::GetProcString( pool.GetParamDef(), &pool)<<")";
}

bool ParamPool2SqlProcessor_DBA_Avatar::GetSqlString_Update(ParamPool& pool, std::string& strSql) const
{
	strSql.clear();

	std::stringstream ss;
	const ParamDef* pParamDef = pool.GetParamDef();
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

bool ParamPool2Sqlprocessor_DBA_Avatar_CommonData::GetSqlString_Insert(ParamPool& pool, std::string& strSql) const
{

}

bool ParamPool2Sqlprocessor_DBA_Avatar_CommonData::GetSqlString_Update(ParamPool& pool, std::string& strSql) const
{

}
