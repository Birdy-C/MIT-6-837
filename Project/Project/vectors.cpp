#include "vectors.h"

Vec3f Vec4f::GetVec3f()
{
	if (w() != 0) {
		return Vec3f(x()/w(), y()/w(), z()/w());
	}
	else {
		return Vec3f(x(), y(), z());
	}
}
Vec3f Vec4f::GetVec3fWipe()
{
	return Vec3f(x(), y(), z());
}