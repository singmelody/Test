#include "StdAfx.h"
#include "AvatarSrv.h"

FINISH_FACTORY_ARG0(AvatarSrv)
AvatarSrv::AvatarSrv(void)
{
}


AvatarSrv::~AvatarSrv(void)
{
}

bool AvatarSrv::SetState(int32 nState)
{
	bool b = m_State.SetBit(nState);
	return b;
}
