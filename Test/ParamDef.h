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

	int32 Size() { return m_poolSize; }
	void Size(int32 nSize) { m_poolSize = nSize; }

	int32 ExtraSize() { return m_poolExtraSize; }
	void ExtraSize(int32 nSize) { m_poolExtraSize = nSize; }

	void OrderBySize();
	void RefreshOffset();
	void MatchClassType();

	void InitDftValues( void* pClassObj );	
	
	char* MakeBuffer();
	void FreeBuffer(char* pBuffer);

	bool IsLoadData() { return m_loadData; }
	int32 GetMaxIndex() { return m_maxIdx; }
protected:
	bool CheckClassType( int32 nValue, const char* pClassType, char* pClassTypeEx);

	int32		m_index;
	std::string m_name;

	std::string m_classParamSet;
	std::string m_classParamSetEx;

	bool  m_loadData;

	int32 m_maxIdx;
	int32 m_count;
	int32 m_poolSize;
	int32 m_poolExtraSize;
	Allocator*	m_pBufferAlloc;

	std::map<UtilID, ParamBase*>	m_paramMap;
	std::vector<ParamBase*>			m_paramList;
	std::vector<ParamBase*>			m_paramListByID;
};

