#include "StdAfx.h"
#include "PeerModuleBase.h"


PeerModuleBase::PeerModuleBase(void)
{
}


PeerModuleBase::~PeerModuleBase(void)
{
}

bool PeerModuleBase::Init(int32 argc, char* argv[])
{
	if( !ModuleBase::Init( argc, argv))
		return false;

	if(!PeerInit())
		return false;

	if( m_bUseConnectionThread)
		StartConnectionThread();

	return true;
}
