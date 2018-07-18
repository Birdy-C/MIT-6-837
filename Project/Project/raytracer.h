#pragma once
#include "ray.h"
#include "hit.h"
class RayTracer
{
	// store the message needed in info.h
public:
	RayTracer();
	~RayTracer();
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
		float indexOfRefraction, Hit &hit) const;

};

