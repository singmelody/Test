#pragma once

#include "PacketData.h"

typedef int8 FSMStateID;

class FSMBasicData : public PacketData
{
public:
	enum
	{
		eNormal			= 0,
		eAvatarIDOnly	= 1,
	};

	enum
	{
		eAdd			= 0,
		eRemove			= 1,
		eUpdate			= 2,
	};

	FSMBasicData(void);
	virtual ~FSMBasicData(void);

	virtual bool SerializeData( uint16 nSyncFlag, MemoryIOStreamBase& ios);

	void ReadPacketData( PacketFSMBase* pPkt);
	void WritePacketData( PacketFSMBase* pPkt);

	int32 nStateAvatarID;

	FSMStateID		StateID;
	FSMStateID		StateCommonData;
	FSMStateID		StateKernalID;

	Vec3			CurPos;
	Vec3			CurDir;

	FSMBasicData& operator=(const FSMBasicData& other);
};

