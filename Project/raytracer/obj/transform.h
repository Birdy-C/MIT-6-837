#pragma once
#include "object3d.h"

class Transform :
	public Object3D
{
	Matrix mat;
	Object3D *obj;
public:
	Transform(Matrix &m, Object3D *o);
	~Transform();
	bool intersect(const Ray &r, Hit &h, float tmin);
	void paint(void);
    void insertIntoGrid(Grid *g, Matrix *m);
};

