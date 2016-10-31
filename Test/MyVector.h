#pragma once
struct float2
{
	float2():x(0),y(0){}
	float2(float _x, float _y) : x(_x),y(_y){}
	float x,y;

	void Normalize();

	float Cross(const float2& v){
		return x*v.y - y*v.x;
	}
};

float2 operator-(const float2& op0, const float2& op1);

struct float3
{
	float3():x(0),y(0),z(0){}
	float3( float _x, float _y, float _z): x(_x),y(_y),z(_z) {}
	float x,y,z;

	void zero() { x=y=z=0; }
	float3& Set(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
		return *this;
	}
};

struct ShapeLine2D
{
	float2 p0,p1;
};

typedef float2 Point2;
typedef float2 Vector2;

typedef float3 Point3;
typedef float3 Vector3;