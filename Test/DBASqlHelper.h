#pragma once

#include <string>
class ParamBase;
class ParamDef;
class ParamPool;

class DBASqlHelper
{
public:
	DBASqlHelper(void);
	~DBASqlHelper(void);

	static void GetInsertString( ParamPool* pParamSet, ParamBase* pParam, 
		std::stringstream& sqlTemplateHead, std::stringstream& sqlTemplateBack);

	static void GetInsertString( ParamDef* pDef, ParamPool* pParamPool, 
		std::stringstream& sqlTemplateHead, std::stringstream& sqlTemplateBack, std::string& res);

	static std::string GetProcString( ParamDef* pDef, ParamPool* pPool, bool bForInsert = true);
	
	static bool GetUpdateSqlString( ParamPool* pParamSet, ParamBase* pParam, std::stringstream& sqlTemplate);
	static bool GetUpdateSqlString( ParamDef* pParamDef, ParamPool* pParamSet, std::string& sql);
};

void sParam2String( ParamPool* pParamSet, ParamBase* pParam, std::stringstream& sqlTemplate);