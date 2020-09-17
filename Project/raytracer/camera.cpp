#include "stdafx.h"
#include <cmath>
#include "camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{

}


OrthographicCamera::OrthographicCamera(Vec3f& tcenter, Vec3f &tdirection, Vec3f& tup, float tsize)
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

PerspectiveCamera::PerspectiveCamera(Vec3f & tcenter, Vec3f & tdirection, Vec3f & tup, float tangle)
{
	center = tcenter;
	direction = tdirection;
	horizontal.Cross3(horizontal, tup, tdirection);
	up.Cross3(up, horizontal, tdirection);
	up.Normalize();
	horizontal.Normalize();
	direction.Normalize();

	angle = tangle;

}

PerspectiveCamera::~PerspectiveCamera()
{

}


Ray PerspectiveCamera::generateRay(Vec2f point)
{
	////？？你的alpha是啥
	//float m = 1.0 / tan(angle);
	//float tanalpha = 1 / sqrt(m*m - 1);
	//Vec3f temp = direction + tanalpha * up * (point.x() - 0.5) + tanalpha * horizontal * (point.y() - 0.5);
	//这里为了之后求深度值考虑，不做归一化
	//temp.Normalize();
	Vec3f temp = direction + 2 * tan(angle / 2) * up * (point.x() - 0.5) + 2 * tan(angle / 2) * horizontal * (point.y() - 0.5);
	return Ray(center, temp);
}


float PerspectiveCamera::getTMin() const
{
	return 0.0f;
}
