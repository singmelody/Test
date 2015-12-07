#pragma once
#include "Singleton.h"
#include "LoadTemplate.h"
#include <map>
#include "ParamDef.h"

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
	ParamMap m_paramDefMap;

	std::string m_paramDefClassType[eParam_Count];
};

