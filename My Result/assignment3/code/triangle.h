#pragma once
#include "object3d.h"
class Triangle:
	public Object3D
{
	Vec3f a, b, c;
	Vec3f normal;
public:
	Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);
	~Triangle();
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	void paint(void);
};

