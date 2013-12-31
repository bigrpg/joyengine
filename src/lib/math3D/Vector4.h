#ifndef __VECTOR4__H__
#define __VECTOR4__H__

#include "math_type_base.h"
#include "Vector3.h"

class Vector4: public Vector4Base
{
public:
	float DotProduct(const Vector4& v) const;

	float Length() const;
	float LengthSquared() const ;
	void Normalize();

	void Scale(float s);
	void Scale(const Vector4& v,float s);

	void Set(float x,float y,float z,float w);
	void SetZero();

	Vector4(float x,float y,float z,float w);
	Vector4(const Vector3& v,float w);
	Vector4(const Vector4& v);
	Vector4();

	//OUTCODE GetOutCode(const Matrix& totalMatrix);

	void operator += ( const Vector4& v);
	void operator -= ( const Vector4& v);
	void operator *= ( const Vector4& v);
	void operator /= ( const Vector4& v);
	void operator /= ( float r);
	void operator *= ( float s);
	float operator [](int index);

	static const Vector4 & Zero()	{ return s_zero; }

protected:
	static Vector4 s_zero;
};

Vector4 operator +( const Vector4& v1, const Vector4& v2 );
Vector4 operator -( const Vector4& v1, const Vector4& v2 );
Vector4 operator *( const Vector4& v1, const Vector4& v2 );
Vector4 operator /( const Vector4& v1, const Vector4& v2 );
Vector4 operator *( const Vector4& v, float s );
Vector4 operator *( float s, const Vector4& v );
Vector4 operator /( const Vector4& v, float s );
bool operator   ==( const Vector4& v1, const Vector4& v2 );
bool operator   !=( const Vector4& v1, const Vector4& v2 );


#endif //__VECTOR4__H__