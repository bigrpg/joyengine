#ifndef __VECTOR2__H__
#define __VECTOR2__H__

#include "math_type_base.h"

class Vector2 : public Vector2Base
{
public:
	Vector2();
	Vector2(float a,float b):Vector2Base(a,b) {}
	void SetZero();
	void Set(float a,float b);
	void Scale(const Vector2& v,float s);

	float Length();
	float LengthSquared();

	void Normalize();

	float DotProduct( const Vector2& v ) const;
	float CrossProduct( const Vector2& v ) const;

	void ProjectOnto( const Vector2& v1, const Vector2& v2 );
	Vector2 ProjectOnto( const Vector2 & v ) const;

	void operator += ( const Vector2& v );
	void operator -= ( const Vector2& v );
	void operator *= ( float s );
	void operator /= ( float s );
	float operator [](int index);

	static const Vector2 & Zero()			{ return s_zero; }
protected:
	static Vector2 s_zero;

};

Vector2 operator +( const Vector2& v1, const Vector2& v2 );
Vector2 operator -( const Vector2& v1, const Vector2& v2 );
Vector2 operator *( const Vector2& v, float s );
Vector2 operator *( float s, const Vector2& v );
Vector2 operator *( const Vector2& v1, const Vector2& v2 );
Vector2 operator /( const Vector2& v, float s );
bool operator   ==( const Vector2& v1, const Vector2& v2 );
bool operator   !=( const Vector2& v1, const Vector2& v2 );

#endif //__VECTOR2__H__