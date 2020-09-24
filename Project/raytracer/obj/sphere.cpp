#include "stdafx.h"
#include <math.h>
#include "sphere.h"
#include "info.h"

const float PI = 3.1415926;
Sphere::Sphere(Vec3f c, float r, Material* m) : center(c), radius(r)
{
	itsMaterial = m;
    itsboundingbox = new BoundingBox(c - Vec3f(r, r, r), c + Vec3f(r, r, r));
}

Sphere::~Sphere()
{
    delete itsboundingbox;
}

bool Sphere::intersect(const Ray & tr, Hit & h, float tmin)
{
    RayTracingStats::IncrementNumIntersections();

    //TODO1
	Vec3f normdir = tr.getDirection();
	float length = tr.getDirection().Length();
	normdir.Normalize();
	Ray r(tr.getOrigin(), normdir);
	float tp;
	Vec3f dir = r.getDirection();
	Vec3f c2c = center - r.getOrigin();
	tp = c2c.Dot3(r.getDirection());
	float d2 = c2c.Dot3(c2c) - tp*tp;
	if (d2 > radius*radius)
	{
		return false;
	}

	float t = sqrt(radius*radius - d2);
	if ((tp - t) / length < tmin)
	{
		if ((tp + t) / length > tmin && (tp + t) / length < h.getT())
		{
			Vec3f intersect(r.pointAtParameter(t + tp) - center);
			intersect.Normalize();
			h.set((tp + t) / length, itsMaterial, intersect, r);
			return true;
		}
		else
			return false;
	}
	else if ((tp - t) / length < h.getT())
	{
		Vec3f point(r.pointAtParameter(tp - t) - center);
		point.Normalize();
		h.set((tp - t) / length, itsMaterial, point, r);
		return true;
	}
	return false;
}

void Sphere::paint(void)
{
    // TODO3
	glSetMaterial();
	float theta = 2 * PI / theta_steps;
	float phi = PI / phi_steps;

	Vec3f *points = new Vec3f[theta_steps * (phi_steps + 1)];

	// initialize all the position for points
	Vec3f top = center + Vec3f(0, 1, 0)*radius;
	Vec3f below = center + Vec3f(0, -1, 0)*radius;

	for (int iPhi = 0; iPhi < phi_steps + 1; iPhi++)
	{
		for (int iTheta = 0; iTheta < theta_steps; iTheta++)
		{
			int index = theta_steps*iPhi + iTheta;
			if (iPhi == 0)
			{
				points[index] = below;
				continue;
			}
			if (iPhi == phi_steps)
			{
				points[index] = top;
				continue;
			}
			float tphi = iPhi * phi - PI / 2;
			float ttheta = theta * iTheta;
			points[index] = center + radius*sin(tphi)*Vec3f(0, 1, 0)
				+ radius*cos(tphi)*cos(ttheta)*Vec3f(1, 0, 0) + radius*cos(tphi)*sin(ttheta)*Vec3f(0, 0, 1);
		}
	}

	glBegin(GL_QUADS);
	for (int iPhi = 0; iPhi < phi_steps; iPhi++)
	{
		for (int iTheta = 0; iTheta < theta_steps; iTheta++)
		{
			//glNormal3f(normal.x(), normal.y(), normal.z());
			int index[4] = { theta_steps * iPhi + iTheta ,theta_steps * iPhi + (iTheta + 1) % theta_steps,
				theta_steps * (iPhi + 1) + (iTheta + 1) % theta_steps ,theta_steps * (iPhi + 1) + iTheta };
			Vec3f normal;
			if (!gouraud)
			{
				Vec3f::Cross3(normal, points[index[0]] - points[index[1]], points[index[2]] - points[index[1]]);
				if (normal.Length() == 0)
				{
					Vec3f::Cross3(normal, points[index[1]] - points[index[2]], points[index[3]] - points[index[2]]);
				}
			}
			for (int i = 0; i < 4; i++)
			{
				if (gouraud)
				{
					normal = points[index[i]] - center;
				}
				normal.Normalize();
				glNormal3f(normal.x(), normal.y(), normal.z());

				glVertex3f(points[index[i]].x(), points[index[i]].y(), points[index[i]].z());
			}
		}
	}

	glEnd();
}

void Sphere::insertIntoGrid(Grid * g, Matrix * m)
{
    // TODO5
    glSetMaterial();
    g->insertMatrix(this, m);

    for (int i = 0; i < g->nx; i++)
    {
        for (int j = 0; j < g->ny; j++)
        {
            for (int k = 0; k < g->nz; k++)
            {
                // 中心到球心的距离加上方形斜边的距离（保证球一定在内部）
                Vec3f blockcenter = g->center(i, j, k);
                Vec3f spherecenter = center;
                float maxlen = radius;
                if (m)
                {
                    m->Transform(spherecenter);
                    Vec3f vec[3] = {Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1)};
                    m->TransformDirection(vec[0]);
                    m->TransformDirection(vec[1]);
                    m->TransformDirection(vec[2]);
                    maxlen *= min(vec[0].Length(), min(vec[1].Length(), vec[2].Length()));
                }
                Vec3f t = blockcenter - spherecenter;

                if (t.Length() < maxlen + g->gethalfdiag() && t.Length() > maxlen - g->gethalfdiag())
                {
                    g->setRecord(i, j, k, this);
                }
            }
        }
    }
}
