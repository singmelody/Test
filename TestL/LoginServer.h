#pragma once
#include "LoginServer.h"

class LoginServer : public LoginBase, public Singleton<LoginServer>
{
public:
	LoginServer(void);
	~LoginServer(void);
};

