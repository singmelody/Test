#pragma once
class Avatar
{
public:
	Avatar(void);
	virtual ~Avatar(void);

	int32 GetAvatarID() { return m_nAvatarID;}
	void SetAvatarID(int32 nID) { m_nAvatarID = nID; }

	int32 GetAvatarDID() { return m_nAvatarDID;}
	void SetAvatarDID(int32 nDID) { m_nAvatarDID = nDID; }

	bool HasAnyComState(uint64 mask) const { return (m_nComState & mask) != 0; }
	bool HasAllComState(uint64 mask) const { return (m_nComState & mask) == mask; }
protected:
	uint64	m_nComState;
	int32	m_nAvatarID;
	int64	m_nAvatarDID;
};

