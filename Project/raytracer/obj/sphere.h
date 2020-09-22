#pragma once
#include "object3d.h"
#include "grid.h"
class Sphere :
	public Object3D
{
	Vec3f center;	
	float radius;
public:
	Sphere(Vec3f c, float r, Material* m);
	~Sphere();
	bool intersect(const Ray &r, Hit &h, float tmin);
	void paint(void);
    void insertIntoGrid(Grid *g, Matrix *m);
};

