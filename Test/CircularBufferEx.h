#pragma once
class CircularBufferEx
{
public:
	CircularBufferEx(void);
	~CircularBufferEx(void);

	void Allocate(uint32 nSize);
	void Remove(uint32 nLen);

	bool Read(void* destionation, uint32 bytes);
	bool Write(const void* data, uint32 bytes);

	inline void* GetBuffer() { return m_dataEnd; }
	inline void* GetBufferStart() { return m_dataStart; }

	inline uint32 GetSpace() { return GetSpaceBeforeData() + GetSpaceAfterData(); }
	inline uint32 GetSize() const { return uint32(m_dataEnd - m_dataStart); }
	inline uint32 GetSpaceBeforeData() const { return uint32(m_dataStart - m_buffer); }
	inline uint32 GetSpaceAfterData() const { return uint32(m_bufferEnd - m_dataEnd); }

	void MoveData2Head();
	void* ReserveForWrite( uint32 bytes, bool bMoveHeadIfNeed = false);
private:
	uint8*	m_buffer;
	uint8*	m_bufferEnd;

	uint8*	m_dataStart;
	uint8*	m_dataEnd;

};

