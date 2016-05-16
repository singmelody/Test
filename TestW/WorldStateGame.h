#pragma once
#include <set>
#include "WorldState.h"

class WorldStateGame : public WorldState
{
public:
	WorldStateGame(void);
	virtual ~WorldStateGame(void);

	virtual void OnNodeCrashed( int32 nSrvID, bool bUseSHM);

protected:
	virtual void OnEnterState( WorldAvatar* pAvatar);
	virtual void OnLeaveState( WorldAvatar* pAvatar);

	virtual void OnPullAvatarDataFinish( WorldAvatar* pAvatar);

	void TryCommissionNodeAvatar( WorldAvatar* pAvatar);

	bool CommisionNodeAvatar( WorldAvatar* pAvatar);
	void DecommisionNodeAvatar( WorldAvatar* pAvatar);

	void PullAvatarDataFromNode( WorldAvatar* pAvatar, bool bLogout);

protected:
	virtual void OnNodeCrashed( int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar){}

	void ReleaseBillingAndDestroy( WorldAvatar* pAvatar);
	void DestroyDummyNodeAvatar( WorldAvatar* pAvatar);
	void TickFCMAvatars();

	std::set<WorldAvatar*>	m_listFCMAvatars;
	int32 m_nAccFCMTickTime;
};
