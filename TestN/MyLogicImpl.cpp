#include "StdAfx.h"
#include "MyLogicImpl.h"

LOGIC_FACTORY_FINISH(MySkillRoot);
Ref_Class(MySkillRoot)
BEHAVIOR_IO(MySkillRoot);
bool MySkillRoot::Process()
{
	if(IsValidMemory(pAvatar))
		return false;

	return true;
}
