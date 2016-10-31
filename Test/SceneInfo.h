#pragma once
#include "DBInterface.h"
#include "MyVector.h"
#include "ID2ItemMap.h"
#include "SceneInstanceMgr.h"

class Scene;

class SceneCreateArg
{
public:
	SceneCreateArg();

	int16	m_SceneSID;
	int32	m_SceneID;
	int32	m_SceneLevel;
	int32	m_NodeSrvID;
	int32	m_ArenaID;

	int64	m_nSceneProcessBits;
	int64	m_nSceneCustomData;
	uint64	m_nRequestTime;

	int16	TemplateID();
	int16	InstanceID();
};


enum EnterType
{
	eEnterType_Default	= 0,
	eEnterType_Succeed	= 1,
	eEnterType_Failed	= 2,
};

enum EnterMode
{
	eEnterMode_PosDir	= 0,	// 按给定方位跳转
	eEnterMode_NoChange	= 1,	// 保持方位不变
};

class PointInfo
{
public:
	PointInfo();

	Vector3		vDir;
	f32			nRange;

	const Vector3& GetPos() const { return vPos; }
	Vector3 GetRangePos() const;

	EnterMode	nEnterMode;
protected:
	Vector3 vPos;
};

class EnterPointInfo : public PointInfo
{
public:
	EnterPointInfo( DBRow& row);

	int32 nSceneID;
	int32 nFromSceneID;
	int32 nEnterType;
};

class EnterPointMap : public ID2ItemMap< uint16, EnterPointInfo>
{};

class EnterInfo
{
public:
	EnterInfo( DBRow& row);
	~EnterInfo();

	int32 nSceneID;
	int32 nFromSceneID;

	bool bDefaultEnter;
	EnterPointMap	Map;
};

class SceneInfo
{
public:
	enum SceneType
	{
		eSceneCreate_MainTrunk		= 0,	// 主场景
		eSceneCreate_TeamCopy		= 1,	// 队伍副本
		eSceneCreate_ScenarioCopy	= 2,	// 剧情副本
		eSceneCreate_TrunkCopy		= 3,	// 主场景位面
		eSceneCreate_RootCopy		= 4,	// 起始副本

		eSceneCreate_WarTrunk		= 5,
		eSceneCreate_GuildCopy		= 6,
		eSceneCreate_TokenCopy		= 7,
		eSceneCreate_GuildWar		= 8,
	};

	SceneInfo(void);
	virtual ~SceneInfo(void);

	static uint16 GetSceneSID(uint32 nSceneID);
	static uint16 GetSceneInstanceID(uint32 nSceneID);

	static uint32 GenSceneID( uint16 nSceneSID, uint16 nInstanceID = 0);

	static const SceneInfo* GetSceneInfo( uint16 nSceneSID);

	bool IsTrunk() const { return m_CreateType == eSceneCreate_MainTrunk || m_CreateType == eSceneCreate_WarTrunk || m_CreateType == eSceneCreate_GuildWar; }

	bool IsMainTrunk() const { return m_CreateType == eSceneCreate_MainTrunk; }

	uint16			m_SceneSID;
	std::string		m_SceneName;
	int32			m_ShowDescript;
	int32			m_ShowName;
	bool			m_bDynEnterSwitch;
	int32			m_nNpcAutoLevelStep;
	uint8			m_CreateType;
	uint16			m_nPlayerMax;
	int32			m_nLoadValue;

};

class SceneInfoEx : public SceneInfo
{
public:
	SceneInfoEx();

	SceneInstanceMgr m_Instances;

	virtual void OnSceneCreate( Scene* pScene);
	virtual void OnSceneDestroy( Scene* pScene);
	virtual Scene* CreateSceneObj();

	uint32 m_nParallelBits;

	std::string m_strSceneClass;
	std::string m_strDftSceneClass;

};

