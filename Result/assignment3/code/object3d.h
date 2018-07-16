#pragma once
#include <iostream>
#include "matrix.h"
#include "material.h"
#include "ray.h"
#include "hit.h"
#include <gl/glut.h>

class Object3D
{
protected:
	Material *itsMaterial;
public:
	Object3D();
	~Object3D();
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void glSetMaterial() const;
	virtual void paint(void) = 0;
};

