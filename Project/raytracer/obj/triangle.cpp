#include "stdafx.h"
#include "triangle.h"
#include "matrix.h"
#include "vectors.h"
#include "grid.h"
#include <algorithm>
#include "raytracing_stats.h"

float det3x3(float a1, float a2, float a3,
	float b1, float b2, float b3,
	float c1, float c2, float c3);

Triangle::Triangle(Vec3f & ta, Vec3f & tb, Vec3f & tc, Material * m)
	:a(ta), b(tb), c(tc)
{
	itsMaterial = m;
	Vec3f::Cross3(normal, tb - ta, tc - tb);
	normal.Normalize();

    if (itsboundingbox)
        delete itsboundingbox;
    itsboundingbox = new BoundingBox(ta, ta);
    itsboundingbox->Extend(tb);
    itsboundingbox->Extend(tc);
}

Triangle::~Triangle()
{
}

bool Triangle::intersect(const Ray & r, Hit & h, float tmin)
{
    RayTracingStats::IncrementNumIntersections();

    // TODO2
	float belta, gamma, t;
	float A;
	A = det3x3(
		a.x() - b.x(), a.x() - c.x(), r.getDirection().x(),
		a.y() - b.y(), a.y() - c.y(), r.getDirection().y(),
		a.z() - b.z(), a.z() - c.z(), r.getDirection().z()
	);
	belta = det3x3(
		a.x() - r.getOrigin().x(), a.x() - c.x(), r.getDirection().x(),
		a.y() - r.getOrigin().y(), a.y() - c.y(), r.getDirection().y(),
		a.z() - r.getOrigin().z(), a.z() - c.z(), r.getDirection().z()
	);
	gamma = det3x3(
		a.x() - b.x(), a.x() - r.getOrigin().x(), r.getDirection().x(),
		a.y() - b.y(), a.y() - r.getOrigin().y(), r.getDirection().y(),
		a.z() - b.z(), a.z() - r.getOrigin().z(), r.getDirection().z()
	);
	t = det3x3(
		a.x() - b.x(), a.x() - c.x(), a.x() - r.getOrigin().x(),
		a.y() - b.y(), a.y() - c.y(), a.y() - r.getOrigin().y(),
		a.z() - b.z(), a.z() - c.z(), a.z() - r.getOrigin().z()
	);
	belta /= A;
	gamma /= A;
	t /= A;
	if (belta > 1 || belta < 0)
		return false;
	if (gamma > 1 || gamma < 0)
		return false;
	if (1 - belta - gamma > 1 || 1 - belta - gamma < 0)
		return false;
	if (t > tmin && t < h.getT())
	{
		h.set(t, itsMaterial, normal, r);
		return true;
	}

	return false;
}

void Triangle::paint(void)
{
    // TODO3
	glSetMaterial();
	glBegin(GL_TRIANGLES);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
}

void Triangle::insertIntoGrid(Grid * g, Matrix * m)
{
    Vec3f vec[3];
    vec[0] = a;
    vec[1] = b;
    vec[2] = c;
    if (m)
    {
        m->Transform(vec[0]);
        m->Transform(vec[1]);
        m->Transform(vec[2]);
    }
    BoundingBox *bb = new BoundingBox(vec[0], vec[0]);
    bb->Extend(vec[1]);
    bb->Extend(vec[2]);

    BoundingBox *gb = g->getBoundingBox();
    Vec3f minimum = bb->getMin() - gb->getMin();
    Vec3f maximum = bb->getMax() - gb->getMin();

    minimum.Divide(g->getBlockSize());
    maximum.Divide(g->getBlockSize());

    maximum += 0.0001;
    minimum -= 0.0001;

    for (int i = max(0.f, floor(minimum.x())); i < min(1.f * g->nx, ceil(maximum.x())); i++)
    {
        for (int j = max(0.f, floor(minimum.y())); j < min(1.f * g->ny, ceil(maximum.y())); j++)
        {
            for (int k = max(0.f, floor(minimum.z())); k < min(1.f * g->nz, ceil(maximum.z())); k++)
            {
                g->setRecord(i, j, k, this);
            }
        }
    }
    delete bb;
}
