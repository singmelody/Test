#include "StdAfx.h"
#include "LoginAction.h"


LoginAction::LoginAction(void)
{
}


LoginAction::~LoginAction(void)
{
}

void LoginAction::DoAction()
{
	TryLoginOne( m_strAccount, m_strPassword);
}

bool LoginAction::TryLoginOne(const std::string& strAccount, const std::string& strPassword)
{
	MyClt* pClt = new MyClt();
	if(!pClt)
		return false;

	pClt->m_strAccount = strAccount + "@" +  WebSites[m_nSelWebSite];
	pClt->m_strPassword = password;
	pClt->m_LoginSrvIP = (LCPTSTR)m_strServerIP;
	pClt->m_LoginSrvPort = m_nPort;

	if(!MyCltManager::Instance().Connect2LoginSrv( pClt, (LCPTSTR)m_strserverIP, m_nport))
	{
		printf("Can't connect server");
		SAFE_DELETE(pClt);
		return;
	}


}
