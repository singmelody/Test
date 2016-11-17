#pragma once
#include "ParamPoolOwner.h"
#include "MyListNode.h"

class GameObject;

typedef MyListNode<GameObject>		TickNode;
typedef MyListNode<GameObject>		*PTICKNODE;
typedef MyList<TickNode>		TickList;

enum GameObjectType
{
	eType_Unkown	= -1,
	eType_Scene		= 0,
	eType_Player	= 1,
};

class GameObject : public virtual ParamPoolOwner
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	int32 GetType() { return m_Type; }
	void SetType(int32 nType) { m_Type = nType; }

	void InitTickNode(GameObject* pObj){ _tick_node.Set(pObj); }
	MyListNode<GameObject>* GetTickNode() { return &_tick_node; }

	virtual bool Tick(int32 nDeltaTime) { return false; }
	virtual void LowTick(int32 nDeltaTime) {}
protected:
	bool		_bDestroy;
	int32		m_Type;

	bool		m_bShow;
	TickNode	_tick_node;
};