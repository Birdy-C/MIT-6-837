#include "plane.h"

const int INF = 100000;

Plane::Plane(Vec3f & tnormal, float td, Material * m)
	:normal(tnormal), d(td)
{
	itsMaterial = m;
	normal.Normalize();

}


Plane::~Plane()
{
}

bool Plane::intersect(const Ray & r, Hit & h, float tmin)
{
	float t = -(-d + normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
	if (t > tmin && t < h.getT())
	{
		h.set(t, itsMaterial, normal, r);
		return true;
	}
	return false;
}

void Plane::paint(void)
{
	//glBegin(GL_QUADS);
	//glNormal3f(normal.x(), normal.y(), normal.z());
	//glVertex3f(x0, y0, z0);
	//glVertex3f(x1, y1, z1);
	//glVertex3f(x2, y2, z2);
	//glVertex3f(x3, y3, z3);
	//glEnd();
}
