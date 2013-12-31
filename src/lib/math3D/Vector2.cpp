#include "Vector2.h"
#include "math_def.h"

Vector2 Vector2::s_zero(0.f,0.f);

void Vector2::SetZero()
{
	x = y = 0.f;
}

Vector2::Vector2() : Vector2Base(0,0)
{

}
void Vector2::Set(float a,float b)
{
	x = a; y = b;
}

void Vector2::Scale(const Vector2& v,float s)
{
	x = v.x * s; y = v.y * s;
}

float Vector2::Length()
{
	return sqrtf(LengthSquared());
}

float Vector2::LengthSquared()
{
	return x*x + y*y;
}

void Vector2::Normalize()
{
	const float length = Length();
	if(length>GSMath::ZERO)
	{
		float r = 1.f/length;
		*this *= r;
	}
}

void Vector2::operator += ( const Vector2& v )
{
	x += v.x; y+= v.y;
}

void Vector2::operator -= ( const Vector2& v )
{
	x -= v.x; y -= v.y;
}

void Vector2::operator *= ( float s )
{
	x *= s; y *= s;
}
	
void Vector2::operator /= ( float s )
{
	float r = 1.f/s;
	*this *= r;
}

float Vector2::DotProduct( const Vector2& v ) const
{
	return x*v.x + y*v.y;
}
	
float Vector2::CrossProduct( const Vector2& v ) const
{
	return x*v.y - y*v.x;
}

float Vector2::operator [](int index)
{
	//ASSERT
	return *(&x+index);
}

Vector2 operator +( const Vector2& v1, const Vector2& v2 )
{
	return Vector2(v1.x+v2.x, v1.y+v2.y);
}

Vector2 operator -( const Vector2& v1, const Vector2& v2 )
{
	return Vector2(v1.x-v2.x, v1.y-v2.y);
}

Vector2 operator *( const Vector2& v, float s )
{
	Vector2 result;
	result.Scale(v,s);
	return result;
}

Vector2 operator *( float s, const Vector2& v )
{
	return v*s;
}
Vector2 operator *( const Vector2& v1, const Vector2& v2 )
{
	return Vector2(v1.x*v2.x,v1.y*v2.y);
}

Vector2 operator /( const Vector2& v, float s )
{
	float r = 1.f/s;
	return Vector2(v.x*r,v.y*r);
}

bool operator   ==( const Vector2& v1, const Vector2& v2 )
{
	return (v1.x==v2.x && v1.y==v2.y);
}

bool operator   !=( const Vector2& v1, const Vector2& v2 )
{
	return (v1.x != v2.x || v1.y != v2.y );
}
