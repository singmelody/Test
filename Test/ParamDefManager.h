#pragma once
#include "Singleton.h"
#include "LoadTemplate.h"
#include <map>
#include "ParamDef.h"
#include "ParamEx.h"
#include "MyString.h"

#define MD5_SIZE 32

class ParamBase;
class ParamDef;
class ParamDefManager;

class ParamDefManager_LoadHelper
{
public:
	bool LoadFromDB( ParamDefManager* pMgr, DBInterface* pDBI);

	bool InitParamDefine( DBInterface* pDBI);
	bool InitParamColumn( DBInterface* pDBI);
	bool InitParamData( DBInterface* pDBI);

};

typedef std::map< int32, ParamDef*> ParamMap;
typedef ParamMap::iterator ParamMapItr;

class ParamDefManager : public LoadTemplate, public Singleton<ParamDefManager>
{

public:
	struct IListener{
		virtual ~IListener(){}
		virtual void OnParamDefLoaded() = 0;
	};

	ParamDefManager(void);
	~ParamDefManager(void);

	virtual bool LoadDataFromDB( DBInterface* pDBI);

	ParamDef* GetParamDef( int32 nParamID, bool bCreate = false);
	void AddParamDef(int32 nIdx, ParamDef* pDef);

	ParamBase* GetParamBase( int32 nParam);
	ParamBase* CreateParam( const char* sParamType, const char* sDft, const char* sMax, const char* sMin);

	const ParamMap& GetDefMap(){ return m_paramDefMap; }
protected:
	void InitParamMD5();
	char sParamMD5[MD5_SIZE];

	ParamMap m_paramDefMap;

	std::string m_paramDefClassType[eParam_Count];
};

template <class T>
void InitValue_A2Num(Param<T>* pParam, const std::string& sDft, const std::string& sMax, T maxValue, const std::string &sMin, T minVal)
{
	T dftValue = MyString::ToValue<T>(sDft.c_str());
	pParam->SetDftValue(dftValue);

	if(!sMax.empty())
	{
		T temp = MyString::ToValue<T>(sMax.c_str());
		if( temp < maxValue )
			maxValue = (T)temp;
	}

	pParam->SetMaxValue(maxValue);

	if(!sMin.empty())
	{
		T temp = MyString::ToValue<T>(sMin.c_str());
		if(temp > minVal)
			minVal = (T)temp;
	}

	pParam->SetMinValue(minVal);
}