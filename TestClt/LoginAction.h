#pragma once
class LoginAction
{
public:
	LoginAction(void);
	~LoginAction(void);

	void DoAction();

protected:
	bool TryLoginOne(const std::string& strAccount, const std::string& strPassword);

	std::string m_strAccount;
	std::string m_strPassword;
};

