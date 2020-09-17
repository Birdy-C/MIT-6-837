#include "sphere.h"


Sphere::Sphere(Vec3f c, float r, Material* m) : center(c), radius(r)
{
	itsMaterial = m;
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray & r, Hit & h, float tmin)
{
	float tp;
	Vec3f dir = r.getDirection();
	Vec3f c2c = center - r.getOrigin();
	tp = c2c.Dot3(r.getDirection());
	float d2 = c2c.Dot3(c2c) - tp*tp;
	if (d2 > radius*radius)
	{
		return false;
	}

	float t = sqrt(radius*radius - d2);
	if (tp - t < tmin)
	{
		if (tp + t > tmin)
		{
			h.set(tp + t, itsMaterial, r);
			return true;
		}
		else
			return false;
	}
	else
	{
		h.set(tp - t, itsMaterial, r);
		return true;
	}
	return true;
}
