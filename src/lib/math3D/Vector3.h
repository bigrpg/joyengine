#ifndef __VECTOR3__H__
#define __VECTOR3__H__

#include "math_type_base.h"

class Vector3 : public Vector3Base
{
public:
	Vector3();
	Vector3(float a,float b,float c):Vector3Base(a,b,c) {}
	void SetZero();
	void Set(float a,float b,float c);
	void Scale(const Vector3& v,float s);

	float Length() const;
	float LengthSquared()const ;

	void Normalize();

	float DotProduct( const Vector3& v ) const;
	Vector3 CrossProduct( const Vector3& v ) const;
	void CrossProduct( const Vector3& v1, const Vector3& v2 );
	
	void ProjectOnto( const Vector3& v1, const Vector3& v2 );
	Vector3 ProjectOnto( const Vector3 & v ) const;

	void operator += ( const Vector3& v );
	void operator -= ( const Vector3& v );
	void operator *= ( float s );
	void operator /= ( float s );
	float operator [](int index);

	static const Vector3 & Zero()			{ return s_zero; }
protected:
	static Vector3  s_zero;

};

Vector3 operator +( const Vector3& v1, const Vector3& v2 );
Vector3 operator -( const Vector3& v1, const Vector3& v2 );
Vector3 operator *( const Vector3& v, float s );
Vector3 operator *( float s, const Vector3& v );
Vector3 operator *( const Vector3& v1, const Vector3& v2 );
Vector3 operator /( const Vector3& v, float s );
bool operator   ==( const Vector3& v1, const Vector3& v2 );
bool operator   !=( const Vector3& v1, const Vector3& v2 );

#endif //__VECTOR3__H__