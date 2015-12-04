#pragma once
#include <string>
#include "DBInterface.h"

class ParamBase;

enum ParamType
{
	eParam_Avatar = 0,
	eParam_Monster = 1,
	eParam_Count
};

class ParamDef
{
public:
	ParamDef(void);
	~ParamDef(void);

	void LoadParamDef( DBRow& row);
	void Init();
	virtual void BindParamDefReferenceTemplates(){}

	bool AddParam( ParamBase* pBase);

	ParamBase* GetParam(int32 nIdx);
	ParamBase* GetParam(const char* pName);

	ParamBase* GetParamByID(int32 nIdx);

	const std::string& ParamSetName() { return m_classParamSet; }

	int32 Index() { return m_index; }
	const std::string& Name() { return m_name; }

protected:
	int32		m_index;
	std::string m_name;

	std::string m_classParamSet;

	int32 m_maxIdx;
	int32 m_count;
	int32 m_poolSize;

	std::map<UtilID, ParamBase*>	m_paramMap;
	std::vector<ParamBase*>			m_paramList;
	std::vector<ParamBase*>			m_paramListByID;
};

