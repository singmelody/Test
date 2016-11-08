#include "StdAfx.h"
#include "PeerSrvModuleBase.h"


PeerSrvModuleBase::PeerSrvModuleBase(SrvType nModuelType)
	: PeerModuleBase(nModuelType)
{
}


PeerSrvModuleBase::~PeerSrvModuleBase(void)
{
}

bool PeerSrvModuleBase::Init(int32 nArgc, char* argv[])
{
	if(!PeerModuleBase::Init( nArgc, argv))
		return false;

	if(!SrvInit())
		return false;

	return true;
}
