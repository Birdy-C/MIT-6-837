#include <cmath>
#include <assert.h>   
#include "object3d.h"
#include "group.h" 
#include "sphere.h"
#include "raytracer.h"
#include "camera.h"
#include "light.h"
#include "info.h"
#include "rayTree.h"
#include "scene_parser.h"
#include "raytracing_stats.h"

const float EPSILON = 0.001;

Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming)
{
    // TODO4
	Vec3f normal_t = normal, incoming_t = incoming;
	normal_t.Normalize();
	incoming_t.Normalize();
	float t = incoming_t.Dot3(normal_t);
	return -2 * t * normal_t + incoming_t;
}

bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming,
	float index_i, float index_t, Vec3f &transmitted)
{
    // TODO4
	Vec3f normal_t = normal, incoming_t = incoming;
	normal_t.Normalize();
	incoming_t.Normalize();
	float t = incoming_t.Dot3(normal_t);
	Vec3f m = -t*normal_t + incoming_t;
	float insin = m.Length();
	float outsin = insin*index_i / index_t;

	if (abs(outsin) > 1)
		return false;

	float outcos = sqrt(1 - outsin*outsin);
	m.Normalize();
	if (t < 0)
		transmitted = -outcos * normal_t + outsin * m;
	else
		transmitted = outcos * normal_t + outsin * m;

	return true;

}


RayTracer::RayTracer(Grid * g)
    :grid(g)
{
}

RayTracer::~RayTracer()
{
}

Vec3f RayTracer::traceRay(Ray & ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit & hit) const
{
	if (bounces > max_bounces)
	{
		return Vec3f(0, 0, 0);
	}

	Object3D *itemAll = mainapp->getGroup();
    if (visualize_grid && grid)
    {
        itemAll = grid;
        bounces = max_bounces;
    }

    RayTracingStats::IncrementNumNonShadowRays();
	if (itemAll->intersect(ray, hit, tmin))
	{
		if (bounces == 0)
			RayTree::SetMainSegment(ray, 0, hit.getT());
		assert(NULL != hit.getMaterial());
		Vec3f Color;
		Color = mainapp->getAmbientLight()* hit.getMaterial()->getDiffuseColor(); // add Ambient Light Out the material

		// Calculate direct light
		for (int lighti = 0; lighti < mainapp->getNumLights(); lighti++)
		{
			Light *light = mainapp->getLight(lighti);
			Vec3f dirToLight, lightColor;
			float distance;
			light->getIllumination(hit.getIntersectionPoint(), dirToLight, lightColor, distance);
			//if (dirToLight.Dot3(ray.getDirection()) < 0)
			//	continue;
			// sending rays toward each light source
			Vec3f mask = Vec3f(1.0, 1.0, 1.0);
			if (shadows)
			{
                RayTracingStats::IncrementNumShadowRays();
				Ray lightTestRay(hit.getIntersectionPoint(), dirToLight);
				Hit lightTestHit;
				if (itemAll->intersect(lightTestRay, lightTestHit, EPSILON))
				{
					float getT = lightTestHit.getT();
					if (getT < distance)
					{
						RayTree::AddShadowSegment(lightTestRay, 0, getT);
						continue;
					}
					else
					{
						RayTree::AddShadowSegment(lightTestRay, 0, distance);
					}
					if (useTransparentShadows)
					{
						float lastT = -1;
						Vec3f lastColor = Vec3f(-1, -1, -1);
						do {
							getT = lightTestHit.getT();
							if (lastColor == lightTestHit.getMaterial()->getTransparentColor())
							{
								Vec3f mask_temp = Vec3f(1, 1, 1)*(1 - (getT - lastT)) + lightTestHit.getMaterial()->getTransparentColor()*(getT - lastT);//线性衰减
								Vec3f::Mult(mask, mask, mask_temp);
							}
							// Hit的t要设置为无穷大。
							lightTestHit.initialize();
							lastT = getT;
							lastColor = lightTestHit.getMaterial()->getTransparentColor();
						} while (itemAll->intersect(lightTestRay, lightTestHit, getT + EPSILON));
					}
				}
				else
				{
					RayTree::AddShadowSegment(lightTestRay, 0, 20);
				}
			}
			Color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor) * mask;
		}

		// Calculate Reflection
		if (bounces < max_bounces && hit.getMaterial()->getRedlectiveColor().Length() > 0)
		{

			Ray reflectRay(hit.getIntersectionPoint(), mirrorDirection(hit.getNormal(), ray.getDirection()));
			Hit reflectHit;
			Color += traceRay(reflectRay, EPSILON, bounces + 1, weight, indexOfRefraction, reflectHit) * hit.getMaterial()->getRedlectiveColor();
			RayTree::AddReflectedSegment(reflectRay, 0, reflectHit.getT());
		}

		// Calculate Transparency
		if (bounces < max_bounces && hit.getMaterial()->getTransparentColor().Length() > 0)
		{
			Hit transHit;
			Vec3f transDir;
			bool inside = hit.getNormal().Dot3(ray.getDirection()) > 0;
			bool check = false;
			if (inside)
				check = transmittedDirection(hit.getNormal(), ray.getDirection(), indexOfRefraction, 1.0, transDir);
			else
				check = transmittedDirection(hit.getNormal(), ray.getDirection(), 1.0, hit.getMaterial()->getIndexOfRef(), transDir);

			Ray transRay(hit.getIntersectionPoint(), transDir);
			if (check)
			{
				if (inside)
					Color += traceRay(transRay, EPSILON, bounces + 1, weight, 1.0, transHit) * hit.getMaterial()->getTransparentColor();
				else
					Color += traceRay(transRay, EPSILON, bounces + 1, weight, hit.getMaterial()->getIndexOfRef(), transHit) * hit.getMaterial()->getTransparentColor();

				RayTree::AddTransmittedSegment(transRay, 0, transHit.getT());
			}
		}
		return Color;
	}
	else if (bounces == 0)
	{
		RayTree::SetMainSegment(ray, 0, 10);
		// if no intersect, return the backgroug Color
	}
	return mainapp->getBackgroundColor();
}
