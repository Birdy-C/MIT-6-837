#include "camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{

}


OrthographicCamera::OrthographicCamera(Vec3f tcenter, Vec3f tdirection, Vec3f tup, int tsize)
{
	center = tcenter;
	direction = tdirection;
	size = tsize;
	horizontal.Cross3(horizontal, tup, tdirection);
	up.Cross3(up, horizontal, tdirection);
	up.Normalize();
	horizontal.Normalize();
	direction.Normalize();

}

OrthographicCamera::~OrthographicCamera()
{
}

// point vary from (0,0) to (1,1)
Ray OrthographicCamera::generateRay(Vec2f point)
{
	Vec3f pos = center + size*up*(point.x() - 0.5) + size*horizontal*(point.y() - 0.5);
	return Ray(pos, direction);
}

float OrthographicCamera::getTMin() const
{
	return -std::numeric_limits<float>::max();
}
