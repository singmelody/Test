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

	int32 GetType() { return m_Type; }
	void SetType(int32 nType) { m_Type = nType; }
protected:
	bool		_bDestroy;
	int32		m_Type;

	bool		m_bShow;
	TickNode	_tick_node;
};