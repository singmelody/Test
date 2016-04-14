#include "StdAfx.h"
#include "TaskImpl.h"
#include "MyMutex.h"

FINISH_FACTORY_ARG0(DBTaskAvatar);

DBTaskAvatar::~DBTaskAvatar()
{
	if(m_bScheduled)
	{
		m_bScheduled = false;

		AutoLock tmpLock(DBAAvatarManagerEx::Instance().GetMutex());
		AvatarSHM* pAvatar = DBAAvatarManagerEx::Instance().GetAvatar();
	}
}

bool DBTaskAvatar::DoAction(DBInterface* pInterface)
{
	if(!IsAvatarInvalid())
	{
		MyLog::error( "DBTaskAvatar::DoAction() Failed! Avatar not valid ! Sql=[%s] avatardid=[%lld]", sqlTemplate.c_str(), nAvatarDID);
	}
}

bool DBTaskAvatar::IsAvatarInvalid()
{

}
