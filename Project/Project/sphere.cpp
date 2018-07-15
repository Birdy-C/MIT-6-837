#include "sphere.h"


Sphere::Sphere(Vec3f c, float r, Material* m) : center(c), radius(r)
{
	itsMaterial = m;
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray & tr, Hit & h, float tmin)
{
	Vec3f normdir = tr.getDirection();
	float length = tr.getDirection().Length();
	normdir.Normalize();
	Ray r(tr.getOrigin(), normdir);
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
	if ((tp - t) / length < tmin)
	{
		if ((tp + t) / length > tmin && (tp + t) / length < h.getT())
		{
			Vec3f intersect(r.pointAtParameter(t + tp) - center);
			intersect.Normalize();
			h.set((tp + t)/ length, itsMaterial, intersect, r);
			return true;
		}
		else
			return false;
	}
	else if((tp - t) / length < h.getT())
	{
		Vec3f point(r.pointAtParameter(tp - t) - center);
		point.Normalize();
		h.set((tp - t) / length, itsMaterial, point, r);
		return true;
	}
	return false;
}

void Sphere::paint(void)
{
}
