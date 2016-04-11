#pragma once

#include "DBTask.h"
#include "Factory.h"
#include <assert.h>

class DBTaskAvatar : public DBTask
{
	DECLARE_FACTORY_ARG0_Ex( DBTaskAvatar, -1, new PoolAllocatorEx, TaskFactory::Instance())
public:
	DBTaskAvatar() : m_bScheduled(false), m_nAvatarStatesNeededBeforeQueue(0) {}
	virtual ~DBTaskAvatar();

	inline void SetScheduled() { assert(!m_bScheduled); m_bScheduled = true; }

protected:
	virtual bool DoAction( DBInterface* pInterface);
	virtual bool IsAvatarInvalid();
	uint8	m_nAvatarStatesNeededBeforeQueue;
	bool	m_bScheduled;
};