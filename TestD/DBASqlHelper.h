#pragma once

#include <string>
class ParamBase;
class ParamDef;


class DBASqlHelper
{
public:
	DBASqlHelper(void);
	~DBASqlHelper(void);

	static void GetInsertString( ParamPool* pParamSet, ParamBase* pParam, 
		std::stringstream& sqlTemplateHead);

	static void GetInsertString( ParamDef* pDef, ParamPool* pParamPool, 
		std::stringstream& sqlTemplateHead, std::stringstream& sqlTemplateBack, std::string& res);

	static std::string GetProcString( ParamDef* pDef, ParamPool* pPool, bool bForInsert = true);
};

void sParam2String( ParamPool* pParamSet, ParamBase* pParam, std::stringstream& sqlTemplate);