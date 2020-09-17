#pragma once
#include "vectors.h"
#include "ray.h"
class Camera
{
protected:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
public:
	Camera();
	virtual ~Camera();

	//The first is used to generate rays for each screen-space coordinate, described as a Vec2f. 
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;

};

class OrthographicCamera :
	public Camera
{
	float size;

public:
	OrthographicCamera(Vec3f &tcenter, Vec3f &tdirection, Vec3f &tup, float tsize);
	~OrthographicCamera();

	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const;
};

class PerspectiveCamera :
	public Camera
{
	float angle;
public:
	PerspectiveCamera(Vec3f &tcenter, Vec3f &tdirection, Vec3f &tup, float tangle);
	~PerspectiveCamera();
	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const;
};
