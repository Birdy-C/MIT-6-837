#pragma once
#include "ray.h"
#include "hit.h"
#include "grid.h"
class RayTracer
{
    Grid *grid;
public:
	RayTracer(Grid *g);
	~RayTracer();
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
		float indexOfRefraction, Hit &hit) const;

};

