#pragma once
#include "ParamPoolOwner.h"
#include "MyListNode.h"

class GameObject;

typedef MyListNode<GameObject>		TickNode;
typedef MyListNode<GameObject>		*PTICKNODE;
typedef MyList<TickNode>		TickList;

class GameObject : public ParamPoolOwner
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	void InitTickNode(GameObject* pObj){ _tick_node.Set(pObj); }
protected:
	bool		_bDestroy;
	int32		m_Type;

	bool		m_bShow;
	TickNode	_tick_node;
};