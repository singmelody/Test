#pragma once

#include ""

class LoginStateManager : public LoginFuncManager, public Singleton<WorldStateManager>
{
public:
	LoginStateManager(void);
	~LoginStateManager(void);

protected:
	static AccountStateBase* s_states[eAccountState_Count];
};

