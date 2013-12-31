#ifndef __MATRIX__H__
#define __MATRIX__H__

#include "math_type_base.h"

class Vector3;
class Vector4;

class Matrix : public MatrixBase
{
public:
	Matrix();

	void SetIdentity();
	void SetZero();

	void SetScale(float x,float y,float z);
	void SetScale(const Vector3& v);

	void SetTranslate( const float x, const float y, const float z );
	void SetTranslate( const Vector3 & pos );

	void SetRotateX(float angle);
	void SetRotateY(float angle);
	void SetRotateZ(float angle);
	void SetRotate(float yaw,float pitch,float roll);

	void Multiply(const Matrix& m1,const Matrix& m2);
	void PreMultiply(const Matrix& m);
	void PostMultiply(const Matrix& m);

	bool Invert(const Matrix& matrix);
	float GetDeterminant() const;

	Vector3 TransformPoint(const Vector3 v);
	void TransformPoint(Vector4& out,const Vector3& in);
	void TransformPoint(Vector4& out,const Vector4& in);

	Vector3 & operator []( int i );
	const Vector3 & operator []( int i ) const;

	Vector4 & Row( int i );
	const Vector4 & Row( int i ) const;

	float Yaw();
	float Pitch();
	float Roll();

	void LookAt( const Vector3& position,const Vector3& direction, const Vector3& up );

	static Matrix s_identity;
};

#endif //__MATRIX__H__