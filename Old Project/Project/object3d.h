#pragma once
#include <iostream>
#include "matrix.h"
#include "material.h"
#include "ray.h"
#include "hit.h"
//#include "grid.h"
#include "boundingbox.h"
#include <gl/glut.h>

//extern class Grid;

class Object3D
{
protected:
	Material *itsMaterial;
	BoundingBox *boundingbox;
	virtual void calculatebox() = 0;
public:
	Object3D();
	~Object3D();
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void glSetMaterial() const;
	virtual void paint(void) = 0;
	virtual BoundingBox* getBoundingBox();
	virtual void insertIntoGrid(class Grid *g, Matrix *m);
};

