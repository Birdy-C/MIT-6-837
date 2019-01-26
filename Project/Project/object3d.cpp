#include "object3d.h"

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
	if (NULL != boundingbox)
	{
		delete boundingbox;
	}
}

void Object3D::glSetMaterial() const
{
	itsMaterial->glSetMaterial();
}

BoundingBox * Object3D::getBoundingBox()
{
	return boundingbox;
}

void Object3D::insertIntoGrid(Grid * g, Matrix * m)
{
	return;
}
