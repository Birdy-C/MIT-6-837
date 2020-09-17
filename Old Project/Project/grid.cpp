#include "grid.h"

Grid::Grid(BoundingBox *bb, int _nx, int _ny, int _nz)
	:boundingbox(bb), nx(_nx), ny(_ny), nz(_nz)
{
	Vec3f r = boundingbox->getMax() - boundingbox->getMin();

	record = new bool[nx * ny * nz];
	memset(record, 0, sizeof(bool) * nx * ny * nz);
	r.Divide(nx, ny, nz);
	halfdiag = r.Length() / 2;

	itsMaterial = new PhongMaterial(Vec3f(1), Vec3f(0), 0);
}

Grid::~Grid()
{
}

bool * Grid::getRecord(int x, int y, int z)
{
	if (x * y * z > nx * ny * nz)
		return nullptr;
	return record + x * (ny * nz) + y * nz + z;
}

Vec3f Grid::center(int x, int y, int z)
{
	Vec3f r = boundingbox->getMax() - boundingbox->getMin();
	return boundingbox->getMin() + Vec3f(r.x() / nx * (0.5 + x), r.y() / ny*(0.5 + y), r.z() / nz*(0.5 + z));
}

void Grid::calculatebox()
{
	assert(false);

}

bool Grid::intersect(const Ray & r, Hit & h, float tmin)
{
	// TODO add intersect
	assert(false);
	return false;
}

void Grid::drawface(int x, int y, int z, int type)
{
	static Vec3f nor[] = { Vec3f(1,0,0),Vec3f(-1,0,0),Vec3f(0,1,0),Vec3f(0,-1,0),
		Vec3f(0,0,1),Vec3f(0,0,-1) };


	Vec3f r = boundingbox->getMax() - boundingbox->getMin();
	Vec3f side = Vec3f(r.x() / nx, r.y() / ny, r.z() / nz);
	side /= 2;
	Vec3f centerpoint = center(x, y, z);
	Vec3f points[] = { Vec3f(1,-1,-1), Vec3f(1,1,-1), Vec3f(-1,1,-1),Vec3f(-1,-1,-1),
		Vec3f(1,-1,1), Vec3f(1,1,1), Vec3f(-1,1,1),Vec3f(-1,-1,1) };
	int index[6][4] = { {0,1,5,4}, {2,3,7,6},{1,2,6,5}, {3,0,4,7} ,{4,5,6,7}, {3,2,1,0} };
	//	glSetMaterial();

	glNormal3f(nor[type].x(), nor[type].y(), nor[type].z());

	for (int i = 0; i < 4; i++)
	{
		Vec3f t = points[index[type][i]];
		t = t * side + centerpoint;
		glVertex3f(t.x(), t.y(), t.z());
	}

}


void Grid::paint(void)
{
	glSetMaterial();
	// TODO delete useless face
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				if (*(getRecord(i, j, k)))
				{
					glBegin(GL_QUADS);
					for (int t = 0; t < 6; t++)
					{
						drawface(i, j, k, t);
					}
					glEnd();
				}
			}
		}
	}

}

BoundingBox * Grid::getBoundingBox()
{
	return boundingbox;
}

void Grid::initializeRayMarch(MarchingInfo & mi, const Ray & r, float tmin) const
{
	// TODO 天呐这个怎么写啊
	auto temp = boundingbox->getMax() - boundingbox->getMin();
	mi.tmin = tmin;
	// ray-box intersection
	float tempmin = boundingbox->intersect( &r, tmin);

	//  Make sure to treat all three intersection cases: when the origin is inside the grid, when it is outside and the ray hits the grid, and when it is outside and it misses the grid. 

	//mi.d_tx = temp.x() / r.getDirection().x;
	//mi.d_ty = temp.y() / r.getDirection().y;
	//mi.d_tz = temp.z() / r.getDirection().z;

}

void Grid::setRecord(int x, int y, int z)
{
	*getRecord(x, y, z) = true;
}
