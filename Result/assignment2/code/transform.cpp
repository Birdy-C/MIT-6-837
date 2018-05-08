#include "transform.h"
#include "matrix.h"

Transform::Transform(Matrix & m, Object3D * o)
	:mat(m),obj(o)
{
}

Transform::~Transform()
{

}

bool Transform::intersect(const Ray & r, Hit & h, float tmin)
{
	Vec4f newOrigin(r.getOrigin(), 1.0);
	Vec4f newDirection(r.getDirection(), 0.0);
	Matrix temp(mat);
	temp.Inverse(temp, 0.001);
	newOrigin *= temp;
	newDirection *= temp;
	Vec3f newDirection3 = newDirection.GetVec3f();
	//newDirection3.Normalize();
	Vec3f newOrigin3 = newOrigin.GetVec3f();
	if (obj->intersect(Ray(newOrigin3,newDirection3), h, tmin))
	{
		Vec4f newNormal(h.getNormal(), 0.0);
		temp.Transpose();
		newNormal *= temp;
		Vec3f m= newNormal.GetVec3fWipe();
		m.Normalize();
		h.set(h.getT(), h.getMaterial(), m, r);
		return true;
	}

	return false;
}
