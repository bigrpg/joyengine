#include "plane.h"

Plane::Plane()
{
	normal_.SetZero();
	d_ = 0.f;
}

Plane::Plane(const Vector3& normal,float d)
{
	normal_ = normal;
	normal_.Normalize();
	d_ = d;
}

Plane::Plane(const Vector3& normal,const Vector3& point)
{
	Form(normal,point);
}

Plane::Plane(const Vector3& p0,const Vector3&p1,const Vector3& p2)
{
	Form(p0,p1,p2);
}

void Plane::Form(const Vector3& normal,const Vector3& point)
{
	normal_ = normal;
	normal_.Normalize();
	d_ = -normal_.DotProduct(point);
}

void Plane::Form(const Vector3& p0,const Vector3& p1,const Vector3&p2)
{
	Vector3 v1 = p1-p0;
	Vector3 v2 = p2-p0;
	normal_ = v1.CrossProduct(v2);
	normal_.Normalize();
	d_ = -normal_.DotProduct(p0);
}

float Plane::PointDistance(const Vector3& p)
{
	return normal_.DotProduct(p)+d_;
}
bool Plane::PointIsFront(const Vector3& p)
{
	return normal_.DotProduct(p) > -d_ ;
}
