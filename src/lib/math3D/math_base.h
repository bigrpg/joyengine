#ifndef __MATH_BASE_CLASS__H__
#define __MATH_BASE_CLASS__H__

#include <math.h>

inline bool almostZero(float a,float b)
{
	return (fabs(a-b) < 0.000001);
}


struct Vector2Base
{
	Vector2Base() {};
	Vector2Base( float _x, float _y ) : x( _x ), y( _y )
	{
	}

	operator float *()				{ return (float *)&x; }
	operator const float *() const	{ return (float *)&x; }

	float x, y;
};

/**
 * This class implements a vector of 3 floats for non-windows platforms.
 */
struct Vector3Base
{
	Vector3Base() {};
	Vector3Base( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z )
	{
	}

	operator float *()				{ return (float *)&x; }
	operator const float *() const	{ return (float *)&x; }

	float x, y, z;
};

/**
 * This class implements a vector of 4 floats for non-windows platforms.
 */
struct Vector4Base
{
	Vector4Base() {};
	Vector4Base( float _x, float _y, float _z, float _w ) :
		x( _x ), y( _y ), z( _z ), w( _w )
	{
	}

	operator float *()				{ return (float *)&x; }
	operator const float *() const	{ return (float *)&x; }

	float x, y, z, w;
};

/**
 * This class implements a 4x4 Matrix for non-windows platforms.
 */
struct MatrixBase
{
	operator float *()				{ return (float *)&m00; }
	operator const float *() const	{ return (float *)&m00; }

	union
	{
		float m[4][4];
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
	};
};

/**
 * This class implements a Quaternion for non-windows platforms.
 */
struct QuaternionBase
{
	QuaternionBase() {};
	QuaternionBase( float _x, float _y, float _z, float _w ) :
		x( _x ), y( _y ), z( _z ), w( _w )
	{
	}

	operator float *()				{ return (float *)&x; }
	operator const float *() const	{ return (float *)&x; }

	float x, y, z, w;
};


#endif //__MATH_BASE_CLASS__H__