#pragma once
class TreeNode
{
public:
	TreeNode(void);
	virtual ~TreeNode(void);

	void AddChild(TreeNode* pNode);
	void ActiveChild(bool bActive = true)
	{
		m_bChildActive = bActive;
	}

	inline bool IsChildActive() { return m_bChildActive; }
	TreeNode* GetParent() { return m_pParent; }

	TreeNode* GetNextChild(TreeNode* pNode);
	TreeNode* GetPrevChild(TreeNode* pNode);
	
	void RemoveChild(TreeNode* pNode);
	void RemoveAllChild();

	void AddBrother(TreeNode* pNode);
	TreeNode* RemoveThisNode();
protected:
	bool		m_bChildActive;
	TreeNode*	m_pChildSlot;
	TreeNode*	m_pPrev;
	TreeNode*	m_pNext;
	TreeNode*	m_pParent;
};

