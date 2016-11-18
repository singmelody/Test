#include "StdAfx.h"
#include "WorldServerNormal.h"
#include "ConfigManager.h"
#include "ParamPool.h"
#include "MyLog.h"

WorldServerNormal::WorldServerNormal(void)
{
	m_nGateCount = 1;
	m_bWarWorld  = false;

	m_nConnectWarState = eCWS_Waiting;
	m_nConnectWarInterval = 5000;

	m_bUseConnectionThread = true;
}


WorldServerNormal::~WorldServerNormal(void)
{
}

void WorldServerNormal::OnConfigLoaded()
{
	WorldServer::OnConfigLoaded();

	ConfigManager::GetConfigValue( "WorldConfig/Cluster", "GateCount", m_nGateCount);

	ConfigManager::GetConfigValue("CommonConfig", "WarIP", m_WarIP);
	ConfigManager::GetConfigValue("CommonConfig", "WarPort", m_WarPort);

	WORLDDOG_SET_VALUE( WarWorld, 0);

	if( Servers.m_nGrpID <= 0 || Servers.m_nGrpID > eSrvID_MaxGrpID)
	{
		MyLog::error("WorldServerNormal::OnConfigLoaded() Bad SrvGrpID=[%d]", Servers.m_nGrpID);
		return;
	}

	int32 nSrvID = Servers.MakeSrvID( eSrv_World, 0);
	SetSrvID(nSrvID);
}
