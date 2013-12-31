#ifndef __PLANE__H__
#define __PLANE__H__

#include "Vector3.h"

//class is plane
//equation is : Ax + By + Cz + d = 0
//(A,B,C) is normalize normal, so Ax' + By' + Cz' + d is the distance of between (x',y',z') and plane
// distance > 0 : (x',y',z') is on front of plance 
class Plane
{
protected:
	Vector3		normal_;
	float		d_;

public:
	Plane();
	Plane(const Vector3& normal,float d);
	Plane(const Vector3& normal,const Vector3& point);
	Plane(const Vector3& p0,const Vector3&p1,const Vector3& p2);
	void Form(const Vector3& normal,const Vector3& point);
	void Form(const Vector3& p0,const Vector3& p1,const Vector3&p2);

	float PointDistance(const Vector3& p);
	bool PointIsFront(const Vector3& p);

};

#endif //__PLANE__H__