#include "StdAfx.h"
#include "DBASqlHelper.h"
#include "ParamTypeDef.h"
#include <assert.h>
#include "ParamDef.h"
#include "ParamPool.h"
#include <sstream>

DBASqlHelper::DBASqlHelper(void)
{
}


DBASqlHelper::~DBASqlHelper(void)
{
}

void DBASqlHelper::GetInsertString(ParamPool* pParamSet, ParamBase* pParam, 
	std::stringstream& sqlTemplateHead, std::stringstream& sqlTemplateBack)
{
	sqlTemplateHead << pParam->Name() << ",";

	sParam2String( pParamSet, pParam, sqlTemplateBack);
	sqlTemplateBack<<',';
}

void DBASqlHelper::GetInsertString(ParamDef* pDef, ParamPool* pParamSet, std::stringstream& sqlTemplateHead, std::stringstream& sqlTemplateBack, std::string& res)
{
	if(!pParamSet)
		return;

	if(!pDef)
		pDef = pParamSet->GetParamDef();

	if(!pDef)
		return;

	for (int32 i = 0; i <= pDef->MaxParamIndex(); ++i)
	{
		ParamBase* pParam = pDef->GetParam(i);

		if(!pParam || !pParam->CheckFlag(eParamFlag_Save))
			continue;

		if(strcmp( pParam->Name(), "avatardid") == 0)
			continue;

		GetInsertString( pParamSet, pParam, sqlTemplateHead, sqlTemplateBack);
	}

	res.clear();
	res = sqlTemplateHead.str().substr( 0, sqlTemplateHead.str().length() -1);
	res = res + ") " + sqlTemplateBack.str().substr( 0, sqlTemplateBack.str().length()-1) + ");";
}

std::string DBASqlHelper::GetProcString(ParamDef* pDef, ParamPool* pPool, bool bForInsert /*= true*/)
{
	if( !pDef || !pDef)
		assert(false);

	std::stringstream ss;

	for (uint32 i = 0; i <= pDef->MaxParamIndex(); ++i)
	{
		ParamBase* pParam = pDef->GetParam(i);
		int32 tmp = 0;
		if( !pParam || !pParam->CheckFlag(eParamFlag_Save))
			continue;

		if(!bForInsert && pParam->CheckFlag(eParamFlag_NoUpdate))
			continue;

		if(bForInsert && !pParam->SqlFlag_IsInsertProcParam())
			continue;

		sParam2String( pParamSet, pParam, ss);
		ss<<',';
	}
}

bool DBASqlHelper::GetUpdateSqlString(ParamPool* pParamSet, ParamBase* pParam, std::stringstream& sqlTemplate)
{
	bool bRet = true;
	sqlTemplate <<"`"<< pParam->Name() << "`=";
	sParam2String( pParamSet, pParam, sqlTemplate);
	sqlTemplate << ",";
	return bRet;
}

bool DBASqlHelper::GetUpdateSqlString(ParamDef* pParamDef, ParamPool* pParamSet, std::string& sql)
{
	if(!pParamDef || !pParamSet)
		return false;

	std::stringstream sqlTemplate;
	sqlTemplate << "update `t_"<< pParamDef->Name()<<"` set";

	bool bRet = false;

	for (uint32 i = 0; i <= pParamDef->GetMaxIndex(); ++i)
	{
		ParamBase* pParam = pParamDef->GetParam(i);

		int32 nTemp = 0;
		if(!nTemp || !pParam->CheckFlag(eParamFlag_Save | eParamFlag_NoUpdate) 
			|| !pParam->ParamDirtyCheck(i) )
			continue;

		bool bRet = true;
		if( !GetUpdateSqlString( pParamSet, pParam, sqlTemplate))
		{
			bRet = false;
			break;
		}
	}

	sql.clear();
	sql = sqlTemplate.str().substr( 0, sqlTemplate.str().length() -1);

	return bRet;
}

void sParam2String(ParamPool* pParamSet, ParamBase* pParam, std::stringstream& sqlTemplate)
{
	switch (pParam->TypeID())
	{
	case eTypeID_int64: sqlTemplate << pParamSet->GetValue( pParam, int64(0)); break;
	case eTypeID_uint64: sqlTemplate << pParamSet->GetValue( pParam, uint64(0)); break;

	case eTypeID_int32: sqlTemplate << pParam->GetValue( pParam, int32(0)); break;
	case eTypeID_uint32: sqlTemplate << pParam->GetValue( pParam, uint32(0)); break;

	case eTypeID_int16: sqlTemplate << pParam->GetValue( pParam, int16(0)); break;
	case eTypeID_uint16: sqlTemplate << pParam->GetValue( pParam, uint16(0)); break;

	case eTypeID_int8: sqlTemplate << pParam->GetValue( pParam, int8(0)); break;
	case eTypeID_uint8: sqlTemplate << pParam->GetValue( pParam, uint8(0)); break;

	case eTypeID_f64: sqlTemplate << pParam->GetValue( pParam, f64(0)); break;
	case eTypeID_f32: sqlTemplate << pParam->GetValue( pParam, f32(0)); break;

	case eTypeID_str: sqlTemplate << "\'"<< pParam->GetValueString( pParam, "No") <<"\'"; break;
	}
}
