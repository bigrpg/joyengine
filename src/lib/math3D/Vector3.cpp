#include "Vector3.h"

Vector3 Vector3::s_zero(0,0,0);

Vector3::Vector3():Vector3Base(0.f,0.f,0.f)
{
}

void Vector3::SetZero()
{
	x = y = z = 0.f;
}

void Vector3::Set(float a,float b,float c)
{
	x = a; y = b; z = c;
}

void Vector3::Scale(const Vector3& v,float s)
{
	x = v.x*s; y = v.y*s; z = v.z*s;
}

Vector3 Vector3::CrossProduct(const Vector3& v) const
{
	return Vector3( y*v.z - z*v.y,
					z*v.x - x*v.z,
					x*v.y - y*v.x);
}

void Vector3::CrossProduct( const Vector3& v1, const Vector3& v2 )
{
	x = (v1.y * v2.z) - (v1.z * v2.y);
	y = (v1.z * v2.x) - (v1.x * v2.z);
	z = (v1.x * v2.y) - (v1.y * v2.x);
}

float Vector3::DotProduct(const Vector3& v) const
{
	return x*v.x + y*v.y + z*v.z;
}

float Vector3::LengthSquared() const
{
	return x*x + y*y + z*z;
}

float Vector3::Length() const
{
	return sqrtf(LengthSquared());
}

void Vector3::Normalize()
{
	const float length = this->Length();

	if (!almostZero( length, 0.000001f ) )
	{
		float rcp = 1.f / length;
		*this *= rcp;
	}

}

Vector3 Vector3::ProjectOnto(const Vector3& v) const
{
	Vector3 nv;
	nv.ProjectOnto(*this,v);
	return nv;
}

void Vector3::ProjectOnto( const Vector3& v1, const Vector3& v2 )
{
	*this = v2 * (v1.DotProduct(v2)/v2.LengthSquared());
}
void Vector3::operator -= ( const Vector3& v)
{
	x -= v.x; y -= v.y; z -= v.z;
}

void Vector3::operator += ( const Vector3& v)
{
	x += v.x; y += v.y; z += v.z;
}

void Vector3::operator *= ( float s)
{
	x *= s; y *= s; z *= s;
}

void Vector3::operator /= (float s)
{
	float r = 1.f/s;
	*this *= r;
}

float Vector3::operator [](int index)
{
	//ASSERT
	return *(&x+index);
}

Vector3 operator +( const Vector3& v1, const Vector3& v2 )
{
	return Vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

Vector3 operator -( const Vector3& v1, const Vector3& v2 )
{
	return Vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

Vector3 operator *( const Vector3& v, float s )
{
	Vector3 result;
	result.Scale(v,s);
	return result;
}

Vector3 operator *( float s, const Vector3& v )
{
	return v*s;
}
Vector3 operator *( const Vector3& v1, const Vector3& v2 )
{
	return Vector3(v1.x*v2.x,v1.y*v2.y,v1.z*v2.z);
}

Vector3 operator /( const Vector3& v, float s )
{
	float r = 1.f/s;
	return Vector3(v.x*r,v.y*r,v.z*r);
}

bool operator   ==( const Vector3& v1, const Vector3& v2 )
{
	return (v1.x==v2.x && v1.y==v2.y && v1.z==v2.z);
}

bool operator   !=( const Vector3& v1, const Vector3& v2 )
{
	return (v1.x != v2.x || v1.y != v2.y || v1.z != v2.z);
}
