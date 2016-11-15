#include "StdAfx.h"
#include "IDPool.h"
#include "MyLog.h"
#include <assert.h>

bool IDPool::id_alloc(uint32& allocid)
{
	if(!free_allocids_.pop(allocid))
	{
		if(cur_max_id_ == value_max_)
		{
			MyLog::warning("Warning IDPool [%s] ID overflow !!!!", pool_des_.c_str());
			return false;
		}
		else
		{
			allocid = ++cur_max_id_;
			assert(allocid != 0);
		}
	}

	return true;
}

void IDPool::free_id(uint32 allocid)
{
	for (int32 i = free_allocids_.start(); i >= 0; i = free_allocids_.next(i))
	{
		if( allocid == free_allocids_.get(i))
		{
			MyLog::warning("Warning IDPool [%s][%d] repeat free !!!", pool_des_.c_str(), allocid);
			return;
		}
	}
	
	free_allocids_.push(allocid);
}
