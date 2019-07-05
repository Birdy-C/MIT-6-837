#include <math.h>
#include "sphere.h"
#include "info.h"

const float PI = 3.1415926;

void Sphere::calculatebox()
{
	boundingbox = new BoundingBox(Vec3f(radius) + center, center - Vec3f(radius));
}

Sphere::Sphere(Vec3f c, float r, Material* m) : center(c), radius(r)
{
	calculatebox();
	itsMaterial = m;
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray & tr, Hit & h, float tmin)
{
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
	glSetMaterial();
	//float theta = 360.0 / theta_steps;
	//float phi = 180.0 / phi_steps;
	float theta = 2 * PI / theta_steps;
	float phi = PI / phi_steps;

	Vec3f *points = new Vec3f[theta_steps * (phi_steps + 1)];

	// initialize all the position for points
	Vec3f top = center + Vec3f(0, 1, 0) * radius;
	Vec3f below = center + Vec3f(0, -1, 0) * radius;

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
			points[index] = center + radius*sin(tphi) * Vec3f(0, 1, 0)
				+ radius*cos(tphi)*cos(ttheta) * Vec3f(1, 0, 0) + radius * cos(tphi) * sin(ttheta) * Vec3f(0, 0, 1);
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
	// Because Grid don't have a settled material, so set it to the material of sphere
	glSetMaterial();

	if (NULL != m)
	{
		this->Object3D::insertIntoGrid(g, m);
		return;
	}

	for (int i = 0; i < g->nx; i++)
	{
		for (int j = 0; j < g->ny; j++)
		{
			for (int k = 0; k < g->nz; k++)
			{
				// 中心到球心的距离加上方形斜边的距离（保证球一定在内部）
				Vec3f t = g->center(i, j, k) - center;
				if (t.Length() < radius + g->gethalfdiag())
				{
					g->setRecord(i, j, k);
				}
			}
		}
	}

}
