#pragma once
#include "object3d.h"
class Sphere :
	public Object3D
{
	Vec3f center;	
	float radius;
	void calculatebox();
public:
	Sphere(Vec3f c, float r, Material* m);
	~Sphere();
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	void paint(void);
	void insertIntoGrid(Grid *g, Matrix *m);

};

