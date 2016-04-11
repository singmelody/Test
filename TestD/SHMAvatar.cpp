#include "StdAfx.h"
#include "SHMAvatar.h"
#include "MyMutex.h"

void AvatarSHM::Sync2DB(bool bExit /*= false*/)
{
	LOCK_THIS_AVATAR();
	if( m_state != eState_Gaming )
		blablabla
}

bool AvatarSHM::ScheduleTask(DBTaskAvatar* pDBTask, DBATaskLevel taskLevel)
{

}
