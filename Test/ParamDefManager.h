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

	bool InitParamDefine( ParamDefManager* pMgr, DBInterface* pDBI);
	bool InitParamColumn( ParamDefManager* pMgr, DBInterface* pDBI);
	bool InitParamData( ParamDefManager* pMgr, DBInterface* pDBI);

};

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
protected:
	void InitParamMD5();

	typedef std::map< int32, ParamDef*> ParamMap;
	typedef ParamMap::iterator ParamMapItr;
	ParamMap m_paramDefMap;

	std::string m_paramDefClassType[eParam_Count];
};

