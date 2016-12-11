#include "StdAfx.h"
#include "NodeScene.h"
#include "GameObject.h"

NodeScene::NodeScene(void)
{
}


NodeScene::~NodeScene(void)
{
}

bool NodeScene::DebugTick(int32 nDeltaTime)
{
	// fast tick
	PTICKNODE pNode = _scene_tick_list.GetNext(NULL);
	while(pNode)
	{
		GameObject* pGameObj = pNode->Get();
		if(!pGameObj)
		{
			pNode = _scene_tick_list.Remove(pNode);
			continue;
		}

		pGameObj->Tick(nDeltaTime);
		pNode = _scene_tick_list.GetNext(pNode);
	}

	// slow tick
	{
		m_nNormalTickTime += nDeltaTime;
		if( m_nNormalTickTime >= NODE_NORMAL_TICK_TIME )
		{
			m_mgrScenarios.Tick(m_nNormalTickTime);

			m_nNormalTickTime -= m_nNormalTickTime;
		}
	}

	// delete npc
	{
		PTICKNODE pNode = _scene_tick_list.GetNext(NULL);
		while (pNode)
		{
			GameObject* pGameObj = pNode->Get();
			if(!pGameObj)
			{
				pNode = _scene_tick_list.Remove(pNode);
				continue;
			}

			if(pGameObj->IsDestroy())
			{
				if( pGameObj->GetType() != eType_Player )
				{
					pNode = _scene_tick_list.GetNext(pNode);

					NodeNPC* pNpc = static_cast<NodeNPC*>(pGameObj);
					if(pNpc)
						ExitSceneAndDestroy(pNpc);

					continue;
				}
				pNode = _scene_tick_list.GetNext(pNode);
			}
		}
	}
}
