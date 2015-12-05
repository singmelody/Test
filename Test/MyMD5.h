#pragma once

typedef unsigned char byte;

#define MD_BUFFER_SIZE 0x40

class MyMD
{
public:
	void Init();
	void Update(const void* pvSrc, uint32 cbSize);
	void Final();

	const byte* GetDigestStream() { return (byte*)m_stContext.digest; }
	int32 GetDiestSize(){ return sizeof(m_stContext.digest); }
private:
	virtual void Transform(const uint32* in) = 0;
protected:
	

	struct STCONTEXT
	{
		uint32 count[2];
		uint32 digest[4];
		byte buffer[MD_BUFFER_SIZE];
	}m_stContext;

	static const byte m_abyPadding[MD_BUFFER_SIZE];
};

class MyMD4 : public MyMD
{
	void Transform(const uint32 *in);
};

class MyMD5 : public MyMD
{
	void Transform(const uint32 *in);
};