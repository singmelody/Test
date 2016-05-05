#pragma once
#include "ParamPoolOwner.h"
#include "MyListNode.h"

class GameObject : public ParamPoolOwner
{
public:
	GameObject(void);
	virtual ~GameObject(void);
};

typedef MyListNode<GameObject*>		TickNode;
typedef std::list<TickNode*>		TickList;