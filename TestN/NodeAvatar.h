#pragma once

class NodeAvatar : public NodeAvatarFSM, public EventProcessorBase<NodeAvatar>, public ISceneObjFinder
{
	DECLARE_FACTORY_ARG0_Ex( NodeAvatar, -1, new PoolAllocator);
public:
	NodeAvatar();
	virtual ~NodeAvatar();
};