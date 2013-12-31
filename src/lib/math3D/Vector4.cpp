#include "Vector4.h"

Vector4 Vector4::s_zero(0.f,0.f,0.f,0.f);

Vector4::Vector4(float x,float y,float z,float w):Vector4Base(x,y,z,w)
{
}

Vector4::Vector4(const Vector3& v,float w):Vector4Base(v.x,v.y,v.z,w)
{
}

Vector4::Vector4(const Vector4&v):Vector4Base(v.x,v.y,v.z,v.w)
{
}

Vector4::Vector4():Vector4Base(0.f,0.f,0.f,0.f)
{
}

void Vector4::SetZero()
{
	x = y = z = w = 0.f;
}

void Vector4::Set(float a,float b,float c,float d)
{
	x = a; y = b; z = c; w = d;
}

void Vector4::Scale(float s)
{
	x *= s; y *= s; z *= s; w *= s;
}

void Vector4::Scale(const Vector4& v,float s)
{
	x = v.x*s; y = v.y*s; z = v.z*s; w = v.w*s;
}

void Vector4::Normalize()
{
	float l = Length();
	l = 1.f/l;
	this->Scale(l);
}

float Vector4::LengthSquared() const
{
	return x*x + y*y + z*z + w*w;
}

float Vector4::Length() const 
{
	float sq = this->LengthSquared();
	return sqrtf(sq);
}

float Vector4::DotProduct(const Vector4& v) const
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

void Vector4::operator -= ( const Vector4& v)
{
	x -= v.x; y -= v.y; z -= v.z; w-= v.w;
}

void Vector4::operator += ( const Vector4& v)
{
	x += v.x; y += v.y; z += v.z; w += v.w;
}

void Vector4::operator *= ( float s)
{
	x *= s; y *= s; z *= s; w *= s;
}

void Vector4::operator *= ( const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void Vector4::operator /= (float s)
{
	float r = 1.f/s;
	*this *= r;
}

void Vector4::operator /= (const Vector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

float Vector4::operator [](int index)
{
	//ASSERT
	return *(&x+index);
}

Vector4 operator +( const Vector4& v1, const Vector4& v2 )
{
	return Vector4(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w);
}

Vector4 operator -( const Vector4& v1, const Vector4& v2 )
{
	return Vector4(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w);
}

Vector4 operator *( const Vector4& v, float s )
{
	Vector4 result;
	result.Scale(v,s);
	return result;
}

Vector4 operator *( float s, const Vector4& v )
{
	return v*s;
}
Vector4 operator *( const Vector4& v1, const Vector4& v2 )
{
	return Vector4(v1.x*v2.x,v1.y*v2.y,v1.z*v2.z,v1.w*v2.w);
}

Vector4 operator /( const Vector4& v, float s )
{
	float r = 1.f/s;
	return Vector4(v.x*r,v.y*r,v.z*r,v.w*r);
}

Vector4 operator /( const Vector4& v, const Vector4& v2 )
{
	return Vector4(v.x/v2.x,v.y*v2.y,v.z*v2.z,v.w*v2.w);
}

bool operator   ==( const Vector4& v1, const Vector4& v2 )
{
	return (v1.x==v2.x && v1.y==v2.y && v1.z==v2.z && v1.w==v2.w);
}

bool operator   !=( const Vector4& v1, const Vector4& v2 )
{
	return (v1.x != v2.x || v1.y != v2.y || v1.z != v2.z || v1.w != v2.w);
}
