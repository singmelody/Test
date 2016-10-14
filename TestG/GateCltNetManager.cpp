#include "StdAfx.h"
#include "GateCltNetManager.h"


GateCltNetManager::GateCltNetManager(bool bLZOCompresss, int32 sockRcBuffSize, int32 rcBuffSize, int32 sockSnBuffSize, int32 snBuffSize, FunctionBase_Arg1<int32>* funcAccept /*= NULL*/, FunctionBase_Arg1<int32>* funcCon /*= NULL*/, FunctionBase_Arg1<int32>* funcDisCon /*= NULL*/, int32 MAX_SOCKETS)
	: BASENETMANAGER( bLZOCompresss, sockRcBuffSize, rcBuffSize, sockSnBuffSize, snBuffSize, funcAccept, funcCon, funcDisCon, MAX_SOCKETS)
{

}

GateCltNetManager::~GateCltNetManager(void)
{

}


NetChannelBase* GateCltNetManager::CreateNewChannel()
{
	return new GateCltNetChannel();
}