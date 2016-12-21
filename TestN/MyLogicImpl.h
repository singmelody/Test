#pragma once

#include "Factory.h"
#include "MyLogic.h"

class MySkillRoot : public MyLogic
{
	LOGIC_FACTORY(MySkillRoot)
	Logic_Ref(MySkillRoot, MyLogic)
	Ref_Class_End(MySkillRoot, MyLogic)
	BEHAVIOR_IO_BEGIN(MySkillRoot)
		REG_OUTPUT_PTR( SkillRoot, pAvatar, ptr)
		REG_OUTPUT_INT( SkillRoot, nSkillID, skill)
		REG_OUTPUT_INT( SkillRoot, nSkillType, skill type)
	BEHAVIOR_IO_END(MySkillRoot)
public:
	MySkillRoot()
	{
		pAvatar = NULL;
		nSkillID = -1;
		nSkillType = 0;

	}

	~MySkillRoot()
	{}

	int32 GetType() { return eLogicType_Skill; }

	bool Process();
	void OnBuffer2Data();
	void OnReset()
	{
		MyLogic::OnReset();
		pAvatar = NULL;
	}

protected:
	NodeAvatar*		pAvatar;
	int32			nSkillID;
	int32			nSkillType;
	int32			nSkillCommonType;
};
