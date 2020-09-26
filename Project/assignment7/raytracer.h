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
    bool RayCast(Object3D *obj, Ray & ray, Hit & hit, float tmin) const;
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
		float indexOfRefraction, Hit &hit) const;

};

