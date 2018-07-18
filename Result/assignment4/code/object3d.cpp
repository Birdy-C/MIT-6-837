#include "object3d.h"

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
}

void Object3D::glSetMaterial() const
{
	itsMaterial->glSetMaterial();
}
