#pragma once
class MyGameActor : public virtual ParamPoolOwner, public virtual MyAvatar
{
public:
	MyGameActor(void);
	virtual ~MyGameActor(void);
};

