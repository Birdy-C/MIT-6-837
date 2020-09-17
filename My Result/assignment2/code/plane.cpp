#include "plane.h"


Plane::Plane(Vec3f & tnormal, float td, Material * m)
	:normal(tnormal), d(td)
{
	itsMaterial = m;
	normal.Normalize();

}


Plane::~Plane()
{
}

bool Plane::intersect(const Ray & r, Hit & h, float tmin)
{
	float t = -(-d + normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
	if (t > tmin && t < h.getT())
	{
		h.set(t, itsMaterial, normal, r);
		return true;
	}
	return false;
}
