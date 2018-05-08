#pragma once
#include <iostream>
#include "matrix.h"
#include "material.h"
#include "ray.h"
#include "hit.h"

class Object3D
{
protected:
	Material *itsMaterial;
public:
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	Object3D();
	~Object3D();
};

