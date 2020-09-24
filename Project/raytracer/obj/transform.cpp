#include "stdafx.h"
#include "transform.h"
#include "matrix.h"

bool Transform::intersectWarp(Object3D * newobj, Matrix matrix, const Ray & r, Hit & h, float tmin)
{
    Vec4f newOrigin(r.getOrigin(), 1.0);
    Vec4f newDirection(r.getDirection(), 0.0);
    Matrix temp(matrix);
    temp.Inverse(temp, 0.001);
    newOrigin *= temp;
    newDirection *= temp;
    Vec3f newDirection3 = newDirection.GetVec3f();
    //newDirection3.Normalize();
    Vec3f newOrigin3 = newOrigin.GetVec3f();
    if (newobj->intersect(Ray(newOrigin3, newDirection3), h, tmin))
    {
        Vec4f newNormal(h.getNormal(), 0.0);
        temp.Transpose();
        newNormal *= temp;
        Vec3f m = newNormal.GetVec3fWipe();
        m.Normalize();
        h.set(h.getT(), h.getMaterial(), m, r);
        return true;
    }
    return false;
}

Transform::Transform(Matrix & m, Object3D * o)
	:mat(m),obj(o)
{
    // TODO5
    if (o->getBoundingBox())
    {
        Vec3f vec[2];
        vec[0] = o->getBoundingBox()->getMin();
        vec[1] = o->getBoundingBox()->getMax();
        itsboundingbox = nullptr;
        for (int i = 1; i < 8; i++)
        {
            Vec3f v = Vec3f(vec[i & 1].x(), vec[(i >> 1) & 1].y(), vec[(i >> 2) & 1].z());
            m.Transform(v);
            if (itsboundingbox)
                itsboundingbox->Extend(v);
            else
                itsboundingbox = new BoundingBox(v, v);
        }
    }
}

Transform::~Transform()
{
    delete obj;
    delete itsboundingbox;
}

bool Transform::intersect(const Ray & r, Hit & h, float tmin)
{
    // TODO2
	Vec4f newOrigin(r.getOrigin(), 1.0);
	Vec4f newDirection(r.getDirection(), 0.0);
	Matrix temp(mat);
	temp.Inverse(temp, 0.001);
	newOrigin *= temp;
	newDirection *= temp;
	Vec3f newDirection3 = newDirection.GetVec3f();
	//newDirection3.Normalize();
	Vec3f newOrigin3 = newOrigin.GetVec3f();
	if (obj->intersect(Ray(newOrigin3,newDirection3), h, tmin))
	{
		Vec4f newNormal(h.getNormal(), 0.0);
		temp.Transpose();
		newNormal *= temp;
		Vec3f m= newNormal.GetVec3fWipe();
		m.Normalize();
		h.set(h.getT(), h.getMaterial(), m, r);
		return true;
	}

	return false;
}


void Transform::paint(void)
{
    // TODO3
	glPushMatrix();
	GLfloat *glMatrix = mat.glGet();
	glMultMatrixf(glMatrix);
	delete[] glMatrix;

	obj->paint();
	glPopMatrix();
}

void Transform::insertIntoGrid(Grid * g, Matrix * m)
{
    if (!m)
    {
        Matrix newm = mat;
        obj->insertIntoGrid(g, &newm);
    }
    else
    {
        Matrix newm = (*m) * mat;
        obj->insertIntoGrid(g, &newm);
    }
}
