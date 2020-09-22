#include "stdafx.h"
#include "object3d.h"

Object3D::Object3D()
    :itsboundingbox(nullptr)
{
}

Object3D::~Object3D()
{
    if (itsboundingbox)
        delete itsboundingbox;
}

void Object3D::glSetMaterial() const
{
	itsMaterial->glSetMaterial();
}
