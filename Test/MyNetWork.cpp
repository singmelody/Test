#include "StdAfx.h"
#include "MyNetWork.h"

NetServiceParam::NetServiceParam()
{
	nNumOfConnThread	= 1;
	nMaxNumOfConn		= 500;
	nIntervalOfTick		= 10;
	nSizeOfSysBuffer	= 65535;
	nMaxSizeOfSysBuffer	= 0;
	nBackLog			= 512;
	nSizeOfConnectThread= 256;
}


void NetServiceParam::Check()
{
	if(nNumOfConnThread < 1)
		nNumOfConnThread = 1;

	if(nMaxNumOfConn < nNumOfConnThread )
		nMaxNumOfConn = nNumOfConnThread;

	if(nIntervalOfTick < 5)
		nIntervalOfTick = 5;

	if(nSizeOfSysBuffer < 2048)
		nSizeOfSysBuffer = 2048;

	if( nMaxSizeOfSysBuffer < nSizeOfSysBuffer * 2)
		nMaxSizeOfSysBuffer = nSizeOfSysBuffer * 2;

	if(nBackLog < 5)
		nBackLog = 5;

	if(nSizeOfConnectThread < 10)
		nSizeOfConnectThread = 0;
}


