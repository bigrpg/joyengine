#include "Matrix.h"
#include "Vector3.h"
#include "Vector4.h"


#include <string.h>
#include <assert.h>


Matrix Matrix::s_identity;

Matrix::Matrix()
{
	SetZero();
}

void Matrix::SetIdentity()
{
	(*this) = Matrix::s_identity;
}

void Matrix::SetZero()
{
	//no virtual function 
	memset(this,0,sizeof(*this));
}

void Matrix::SetScale(float x,float y,float z)
{
	m[0][0] = x;
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = y;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = z;
	m[2][3] = 0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;
}

void Matrix::SetScale(const Vector3& v)
{
	SetTranslate(v.x,v.y,v.z);
}

void Matrix::SetTranslate( const float x, const float y, const float z )
{
	m[0][0] = 1.f;
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = 1.f;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = 1.f;
	m[2][3] = 0.f;

	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	m[3][3] = 1.f;

}
void Matrix::SetTranslate( const Vector3 & pos )
{
	SetTranslate(pos.x,pos.y,pos.z);
}

void Matrix::SetRotateX(float angle)
{
	float sa = (float)sin( angle );
	float ca = (float)cos( angle );

	m[0][0] = 1.f;
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = ca;
	m[1][2] = sa;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = -sa;
	m[2][2] = ca;
	m[2][3] = 0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;}

void Matrix::SetRotateY(float angle)
{
	float sa = (float)sin( angle );
	float ca = (float)cos( angle );

	m[0][0] = ca;
	m[0][1] = 0.f;
	m[0][2] = -sa;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = 1.f;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = sa;
	m[2][1] = 0.f;
	m[2][2] = ca;
	m[2][3] = 0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;
}
void Matrix::SetRotateZ(float angle)
{
	float sa = (float)sin( angle );
	float ca = (float)cos( angle );

	m[0][0] = ca;
	m[0][1] = sa;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = -sa;
	m[1][1] = ca;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = 1.f;
	m[2][3] = 0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;
}

void Matrix::SetRotate(float yaw,float pitch,float roll)
{
	const double sya = sin( yaw );
	const double cya = cos( yaw );
	const double sxa = sin( pitch );
	const double cxa = cos( pitch );
	const double sza = sin( roll );
	const double cza = cos( roll );

	m[0][0] =  cya * cza;
	m[0][1] =  cxa * sza;
	m[0][2] = -sya * cza + cya * sza * sxa;
	m[0][3] =  0.f;

	m[1][0] = -cya * sza;
	m[1][1] =  cxa * cza;
	m[1][2] =  sya * sza + cya * cza * sxa;
	m[1][3] =  0.f;

	m[2][0] =  sya * cxa;
	m[2][1] = -sxa;
	m[2][2] =  cxa * cya;
	m[2][3] =  0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;
}

void Matrix::Multiply(const Matrix& m1,const Matrix& m2)
{
	assert( this != &m1 && this != &m2);
		
	m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30;
	m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31;
	m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32;
	m03 = m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33;

	m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30;
	m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31;
	m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32;
	m13 = m1.m10 * m2.m02 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33;

	m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30;
	m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31;
	m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32;
	m23 = m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33;

	m30 = m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30;
	m31 = m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31;
	m32 = m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32;
	m33 = m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33;
}
void Matrix::PreMultiply(const Matrix& m)
{
	assert( this != &m);
	Matrix tmp( *this );
	Multiply( m, tmp );
}

void Matrix::PostMultiply(const Matrix& m)
{
	assert( this != &m);
	Matrix tmp( *this );
	Multiply( tmp, m );

}

bool Matrix::Invert(const Matrix& matrix)
{
	float determinant = matrix.GetDeterminant();

	if (determinant == 0.f)
	{
		assert(0);
		return false;
	}

	// TODO: Need to consider the invert of a 4x4. This is for a 3x4.

	float rcp = 1 / determinant;

	m[0][0] = matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1];
	m[0][1] = matrix.m[0][2] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][2];
	m[0][2] = matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1];
	m[1][0] = matrix.m[1][2] * matrix.m[2][0] - matrix.m[1][0] * matrix.m[2][2];
	m[1][1] = matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0];
	m[1][2] = matrix.m[0][2] * matrix.m[1][0] - matrix.m[0][0] * matrix.m[1][2];
	m[2][0] = matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0];
	m[2][1] = matrix.m[0][1] * matrix.m[2][0] - matrix.m[0][0] * matrix.m[2][1];
	m[2][2] = matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0];

	m[0][0] *= rcp;
	m[0][1] *= rcp;
	m[0][2] *= rcp;

	m[1][0] *= rcp;
	m[1][1] *= rcp;
	m[1][2] *= rcp;

	m[2][0] *= rcp;
	m[2][1] *= rcp;
	m[2][2] *= rcp;

	m[3][0] = -(matrix.m[3][0] * m[0][0] + matrix.m[3][1] * m[1][0] + matrix.m[3][2] * m[2][0]);
	m[3][1] = -(matrix.m[3][0] * m[0][1] + matrix.m[3][1] * m[1][1] + matrix.m[3][2] * m[2][1]);
	m[3][2] = -(matrix.m[3][0] * m[0][2] + matrix.m[3][1] * m[1][2] + matrix.m[3][2] * m[2][2]);

	if (determinant == 0)
	{
		this->SetIdentity();
	}

	return true;
	
}

float Matrix::GetDeterminant() const
{
	float det = 0;

	det += m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	det -= m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	det += m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

	return det;
}


Vector3 Matrix::TransformPoint(const Vector3 v2)
{
	return Vector3(
		v2.x * m[0][0] + v2.y * m[1][0] + v2.z * m[2][0] + m[3][0],
		v2.x * m[0][1] + v2.y * m[1][1] + v2.z * m[2][1] + m[3][1],
		v2.x * m[0][2] + v2.y * m[1][2] + v2.z * m[2][2] + m[3][2]);
}

void Matrix::TransformPoint(Vector4& v1,const Vector3& v2)
{
	v1.x = v2[0] * m[0][0] + v2[1] * m[1][0] + v2[2] * m[2][0] + m[3][0];
	v1.y = v2[0] * m[0][1] + v2[1] * m[1][1] + v2[2] * m[2][1] + m[3][1];
	v1.z = v2[0] * m[0][2] + v2[1] * m[1][2] + v2[2] * m[2][2] + m[3][2];
	v1.w = v2[0] * m[0][3] + v2[1] * m[1][3] + v2[2] * m[2][3] + m[3][3];
}
void Matrix::TransformPoint(Vector4& v1,const Vector4& v2)
{
	v1.x = v2[0] * m[0][0] + v2[1] * m[1][0] + v2[2] * m[2][0] + v2[3] * m[3][0];
	v1.y = v2[0] * m[0][1] + v2[1] * m[1][1] + v2[2] * m[2][1] + v2[3] * m[3][1];
	v1.z = v2[0] * m[0][2] + v2[1] * m[1][2] + v2[2] * m[2][2] + v2[3] * m[3][2];
	v1.w = v2[0] * m[0][3] + v2[1] * m[1][3] + v2[2] * m[2][3] + v2[3] * m[3][3];
}

Vector3 & Matrix::operator []( int i )
{
	//ASSERT
	return (*(Vector3*)m[i]);
}
const Vector3 & Matrix::operator []( int i ) const
{
	//ASSERT
	return (*(Vector3*)m[i]);
}

Vector4 & Matrix::Row( int i )
{
	return (*(Vector4*)m[i]);
}
const Vector4 & Matrix::Row( int i ) const
{
	return (*(Vector4*)m[i]);
}

float Matrix::Yaw()
{
	const Vector3 & zdir = (*this)[2];
	return atan2f( zdir.x, zdir.z );	
}
float Matrix::Pitch()
{
	const Vector3 & zdir = (*this)[2];
	return -asinf( zdir.y );
}
float Matrix::Roll()
{
	float roll;
	const Vector3 & xdir = (*this)[0];
	const Vector3 & zdir = (*this)[2];

	const float zdirxzlen = sqrtf( zdir.z*zdir.z + zdir.x*zdir.x );
	const float acarg = (zdir.z * xdir.x - zdir.x * xdir.z) / zdirxzlen;
	if (zdirxzlen == 0.f || acarg > 1.f) return 0.f;
	roll = acosf( acarg );
	return xdir.y < 0.f ? -roll : roll;	
}

void Matrix::LookAt( const Vector3& position,const Vector3& direction, const Vector3& up )
{
	Vector3 Up;
	Vector3 Direction( direction );
	Vector3 Right;

	Direction.Normalize( );
	Right.CrossProduct( up, Direction );
	Right.Normalize( );
	Up.CrossProduct( Direction, Right );

	m[0][0] = Right.x;
	m[1][0] = Right.y;
	m[2][0] = Right.z;
	m[3][0] = 0.f;

	m[0][1] = Up.x;
	m[1][1] = Up.y;
	m[2][1] = Up.z;
	m[3][1] = 0.f;

	m[0][2] = Direction.x;
	m[1][2] = Direction.y;
	m[2][2] = Direction.z;
	m[3][2] = 0.f;

	m[3][0] = -position.DotProduct( Right );
	m[3][1] = -position.DotProduct( Up );
	m[3][2] = -position.DotProduct( Direction );
	m[3][3] = 1.f;
}



