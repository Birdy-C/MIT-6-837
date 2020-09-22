#pragma once
#include <iostream>
#include "matrix.h"
#include "material.h"
#include "ray.h"
#include "hit.h"
#include <gl/glut.h>
#include "boundingbox.h"
class Grid;
class Object3D
{
protected:
	Material *itsMaterial;
    BoundingBox *itsboundingbox;
public:
	Object3D();
	~Object3D();
    BoundingBox* getBoundingBox() { return itsboundingbox; };
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void glSetMaterial() const;
	virtual void paint(void) = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m) {};
};

