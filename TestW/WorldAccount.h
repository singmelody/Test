#pragma once

#include "BaseType.h"
#include <string>

class ParamPool;

class WorldAccount
{
public:
	WorldAccount(void);
	~WorldAccount(void);

	ParamPool* CreateRoleSet( int32 nIdx, int32 nType, int32 nData);
	void DestroyRoleSet(int32 nIdx);
	void DestroyAllRoleSet();
	ParamPool* PickRoleSet(int32 nIdx);

	void AddRoleSet( class PacketUserData* pPkt);
	ParamPool* GetRoleSet( int32 nIdx);
	bool SetRoleSet( int32 nIdx, ParamPool* pPool);

	void SetAccountName(const std::string& strAccount);
	const std::string& GetAccountName() const { return m_AccountName; }

	inline bool HasBillingOlOnce() const { return m_bHasBillingOlOnce; }

	void SetRecentRoleSet( ParamPool* pPool);

	void SetUserIp( uint32 nIP) { m_nUserIP = nIP; }
	uint32 GetUserIp() const { return m_nUserIP; }

	void IsFCMAccount(bool b) { m_bIsFCMAcount = b; }
	bool IsFCMAccount()	const { return m_bIsFCMAcount; }

	void NeedNoticeFCMState(bool b) { m_bNeedNoticeFCMStage = b; } 
	bool NeedNoticeFCMState() const { return m_bNeedNoticeFCMStage; }
protected:
	ParamPool*	m_pRoleDataSet[MAX_AVATAR_COUNT_ONE_USER];
	ParamPool*	m_pRecentRoleSet;
	
	std::string	m_AccountName;
	uint32		m_nUserIP;

	bool		m_bHasBillingOlOnce;
	bool		m_bIsBillingOl;

	int32		m_nAccTimeAfterLastHeartBeat;

	bool		m_bIsFCMAcount;
	uint32		m_nMilliSeconds;
	bool		m_bNeedNoticeFCMStage;
};

