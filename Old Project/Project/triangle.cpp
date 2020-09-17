#include "triangle.h"
#include "matrix.h"
#include "vectors.h"

float det3x3(float a1, float a2, float a3,
	float b1, float b2, float b3,
	float c1, float c2, float c3);

void Triangle::calculatebox()
{
	boundingbox = new BoundingBox(a,b,c);
}

Triangle::Triangle(Vec3f & ta, Vec3f & tb, Vec3f & tc, Material * m)
	:a(ta), b(tb), c(tc)
{
	calculatebox();
	itsMaterial = m;
	Vec3f::Cross3(normal, tb - ta, tc - tb);
	normal.Normalize();
}

Triangle::~Triangle()
{

}

bool Triangle::intersect(const Ray & r, Hit & h, float tmin)
{
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
	glSetMaterial();
	glBegin(GL_TRIANGLES);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
}
