#pragma once

#include "GateAccount.h"
#include "Singleton.h"

class GateAccountStateBase
{
protected:
	explicit GateAccountStateBase(GateAccountState state);
	virtual ~GateAccountStateBase(){}
};

class GameAccountState_CreateAvatar : public GateAccountStateBase, public Singleton<GameAccountState_CreateAvatar>
{
public:
	GameAccountState_CreateAvatar(void) : GateAccountStateBase(eGateAccountState_CreatingAvatar){}
	virtual ~GameAccountState_CreateAvatar(void){}

protected:
	virtual const char* GetStateName() const { return "CreatingAvatar"; }
	virtual void OnEnter(GateAccount& account);
};

