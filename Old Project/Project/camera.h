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

	// initializing, placing, dollying, trucking, and rotating an OpenGL camera.
	virtual void glInit(int w, int h) = 0; 
	virtual void glPlaceCamera(void) = 0; 
	virtual void dollyCamera(float dist) = 0; 
	virtual void truckCamera(float dx, float dy) = 0; 
	virtual void rotateCamera(float rx, float ry) = 0;
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
	void glInit(int w, int h);
	void glPlaceCamera(void);
	void dollyCamera(float dist);
	void truckCamera(float dx, float dy);
	void rotateCamera(float rx, float ry);
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
	void glInit(int w, int h);
	void glPlaceCamera(void);
	void dollyCamera(float dist);
	void truckCamera(float dx, float dy);
	void rotateCamera(float rx, float ry);
};

