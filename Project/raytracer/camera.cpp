#include "stdafx.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include <cmath>
#include "matrix.h"
#include "camera.h"

#include <gl/glut.h>

// ========================================================
// Function for Class camera
// ========================================================

Camera::Camera()
{
}


Camera::~Camera()
{

}

// ========================================================
// Function for Class OrthographicCamera
// ========================================================

OrthographicCamera::OrthographicCamera(Vec3f& tcenter, Vec3f &tdirection, Vec3f& tup, float tsize)
{
	center = tcenter;
	direction = tdirection;
	size = tsize;
	Vec3f::Cross3(horizontal, tup, tdirection);
	//Vec3f::Cross3(up, horizontal, tdirection);
	Vec3f::Cross3(up, tdirection, horizontal);
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
    // TODO1
	Vec3f pos = center + size*up*(point.x() - 0.5) + size*horizontal*(point.y() - 0.5);
	return Ray(pos, direction);
}

float OrthographicCamera::getTMin() const
{
	return -std::numeric_limits<float>::max();
}

// Create an orthographic camera with the appropriate dimensions that 
// crops the screen in the narrowest dimension. // 
void OrthographicCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-size / 2.0, size / 2.0, -size*(float)h / (float)w / 2.0,
			size*(float)h / (float)w / 2.0, 0.5, 40.0);
	else
		glOrtho(-size*(float)w / (float)h / 2.0, size*(float)w / (float)h / 2.0,
			-size / 2.0, size / 2.0, 0.5, 40.0);
}

// Place an orthographic camera within an OpenGL scene 
void OrthographicCamera::glPlaceCamera(void)
{
	gluLookAt(center.x(), center.y(), center.z(),
		center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
		up.x(), up.y(), up.z());
}

// dollyCamera, truckCamera, and RotateCamera //
// Asumptions: 
// - up is really up (i.e., it hasn't been changed 
// to point to "screen up") 
// - up and direction are normalized 
// Special considerations:
// - If your constructor precomputes any vectors for
// use in 'generateRay', you will likely to recompute those
// values at athe end of the these three routines //

// dollyCamera: Move camera along the direction vector 
void OrthographicCamera::dollyCamera(float dist) {
	center += direction*dist;
	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values 
	// =========================================== 
}

// truckCamera: Translate camera perpendicular to the direction vector
void OrthographicCamera::truckCamera(float dx, float dy)
{
	//Vec3f horizontal;
	//Vec3f::Cross3(horizontal, direction, up);
	//horizontal.Normalize();
	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);
	center += horizontal*dx + screenUp*dy;
	// =========================================== 
	// ASSIGNMENT 3: Fix any other affected values
	// =========================================== 
}

// rotateCamera: Rotate around the up and horizontal vectors
void OrthographicCamera::rotateCamera(float rx, float ry)
{
	//Vec3f horizontal;
	//Vec3f::Cross3(horizontal, direction, up); 
	//horizontal.Normalize(); // Don't let the model flip upside-down (There is a singularity 
	//						// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13) ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01) ry = tiltAngle - 0.01;
	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
	rotMat.Transform(center);
	rotMat.TransformDirection(direction);
	// =========================================== 
	// ASSIGNMENT 3: Fix any other affected values 
	// ===========================================
    rotMat.TransformDirection(up);
    up.Normalize();
    rotMat.TransformDirection(horizontal);
    horizontal.Normalize();
}


// ========================================================
// Function For Class PerspectiveCamera
// ========================================================

PerspectiveCamera::PerspectiveCamera(Vec3f & tcenter, Vec3f & tdirection, Vec3f & tup, float tangle)
{
	center = tcenter;
	direction = tdirection;
	Vec3f::Cross3(horizontal, tup, tdirection);
	//Vec3f::Cross3(up, horizontal, tdirection);
	Vec3f::Cross3(up, tdirection, horizontal);

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
    // TODO2
	float m = 1.0 / tan(angle / 2);
	float tanalpha = 1 / sqrt(m * m - 1);
	Vec3f temp = direction + 2 * tanalpha * up * (point.x() - 0.5) + 2 * tanalpha * horizontal * (point.y() - 0.5);
	//这里为了之后求深度值考虑，不做归一化
	//temp.Normalize();
	//Vec3f temp = direction + 2 * tan(angle / 2) * up * (point.x() - 0.5) + 2 * tan(angle / 2) * horizontal * (point.y() - 0.5);
	return Ray(center, temp);
}


float PerspectiveCamera::getTMin() const
{
	return 0.0f;
}

// Create a perspective camera with the appropriate dimensions that 
// crops or stretches in the x-dimension as necessary 
void PerspectiveCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle*180.0 / 3.14159,
		(float)w / float(h), 0.5, 40.0);
}

// Place a perspective camera within an OpenGL scene 
void PerspectiveCamera::glPlaceCamera(void)
{
	gluLookAt(center.x(), center.y(), center.z(),
		center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
		up.x(), up.y(), up.z());
}
// ====================================================================
// dollyCamera, truckCamera, and RotateCamera 
// Asumptions: 
// - up is really up (i.e., it hasn't been changed 
// to point to "screen up") 
// - up and direction are normalized 
// Special considerations: 
// - If your constructor precomputes any vectors for
// use in 'generateRay', you will likely to recompute those
// values at athe end of the these three routines 
// ==================================================================== 
// ==================================================================== 
// dollyCamera: Move camera along the direction vector 
// ====================================================================
void PerspectiveCamera::dollyCamera(float dist)
{
	center += direction*dist;
	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values 
	// =========================================== 
}


// truckCamera: Translate camera perpendicular to the direction vector 
void PerspectiveCamera::truckCamera(float dx, float dy)
{
	//Vec3f horizontal;
	//Vec3f::Cross3(horizontal, direction, up);
	//horizontal.Normalize(); 
	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);
	center += horizontal*dx + screenUp*dy;
	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values 
	// =========================================== 
}

// rotateCamera: Rotate around the up and horizontal vectors 
void PerspectiveCamera::rotateCamera(float rx, float ry)
{
	//Vec3f horizontal;
	//Vec3f::Cross3(horizontal, direction, up);
	//horizontal.Normalize();
	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned) 
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13) ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01) ry = tiltAngle - 0.01;
	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
	rotMat.Transform(center);
	rotMat.TransformDirection(direction);
	direction.Normalize();

	// =========================================== 
	// ASSIGNMENT 3: Fix any other affected values 
	// =========================================== 
    rotMat.TransformDirection(up);
    up.Normalize();
    rotMat.TransformDirection(horizontal);
    horizontal.Normalize();
}
