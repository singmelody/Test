#include "StdAfx.h"
#include "FSMBasicData.h"


FSMBasicData::FSMBasicData(void)
{
	StateID = eStateType_Unkown;
	StateCommonData = -1;
	StateKernalID = 0;

	CurPos.zero();
	CurDir.zero();
}


FSMBasicData::~FSMBasicData(void)
{
}

bool FSMBasicData::SerializeData(uint16 nSyncFlag, MemoryIOStreamBase& ios)
{
	ios.SerializeT(nStateAvatarID);

	if(nSyncFlag != eAvatarIDOnly )
	{
		ios.SerializeT(nStateID).SerializeT(nStateKernalID).SerializeT(StateCommonData);
		ios.SerializeF32AsInt16( CurPos, 10.0f);

		if(ios.IsOStream())
		{
			float ang = Dir2Ang_2D(CurDir);
			ios.SerializeF32AsInt16( ang, 10);
		}
		else
		{
			float ang;
			ios.SerializeF32AsInt16( ang, 10);
			CurDir = Ang2Dir_2D(ang);
		}
	}
}
