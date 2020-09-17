#pragma once
#include "object3d.h"
class Sphere :
	public Object3D
{
	Vec3f center;	
	float radius;
public:
	Sphere(Vec3f c, float r, Material* m);
	~Sphere();
	virtual bool intersect(const Ray &r, Hit &h, float tmin);

};

