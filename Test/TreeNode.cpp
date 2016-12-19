#include "StdAfx.h"
#include "TreeNode.h"


TreeNode::TreeNode(void)
	: m_pNext(NULL)
	, m_pPrev(NULL)
	, m_pParent(NULL)
{
	m_bChildActive = true;
	m_pChildSlot = NULL;
}


TreeNode::~TreeNode(void)
{
}

void TreeNode::AddChild(TreeNode* pNode)
{
	if(!pNode)
		return;

	pNode->m_pParent = this;
	if(m_pChildSlot)
	{
		m_pChildSlot->AddBrother(pNode);
		return;
	}
	m_pChildSlot = pNode;
}

TreeNode* TreeNode::GetNextChild(TreeNode* pNode)
{
	if(!pNode)
		return m_pChildSlot;

	return pNode->m_pNext;
}

