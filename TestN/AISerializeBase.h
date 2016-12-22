#pragma once
class AISerializeBase
{
	AIUNIT_Ref_Base(AISerializeBase)
	AIUINT_Ref_End(AISerializeBase)
public:
	AISerializeBase(void);
	virtual ~AISerializeBase(void);

	void SetMemberValue(const char* sName, const char* sValue);
	void SetMemberValue(const char* sString);
};

