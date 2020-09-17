#pragma once
#include "object3d.h"

class Transform :
	public Object3D
{
	Matrix mat;
	Object3D *obj;
	void calculatebox();
public:
	Transform(Matrix &m, Object3D *o);
	~Transform();
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	void paint(void);
};

