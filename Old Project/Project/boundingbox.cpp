#include "boundingbox.h"
#include <gl/glut.h>
#include <algorithm>
// ====================================================================
// ====================================================================

// get ray-box intersection
float BoundingBox::intersect(const Ray * ray, float tmin)
{
	float temp;
	temp = ray->caculatemin(0, min.x());
	tmin = std::min(temp, tmin);
	temp = ray->caculatemin(0, max.x());
	tmin = std::min(temp, tmin);
	temp = ray->caculatemin(1, min.y());
	tmin = std::min(temp, tmin);
	temp = ray->caculatemin(1, max.y());
	tmin = std::min(temp, tmin);
	temp = ray->caculatemin(2, min.z());
	tmin = std::min(temp, tmin);
	temp = ray->caculatemin(2, max.z());
	tmin = std::min(temp, tmin);
	return tmin;
}

void BoundingBox::paint() const {
  // draw a wireframe box to represent the boundingbox
  
  glColor3f(1,1,1);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);

  glVertex3f(min.x(),min.y(),min.z());
  glVertex3f(max.x(),min.y(),min.z());
  glVertex3f(min.x(),min.y(),min.z());
  glVertex3f(min.x(),max.y(),min.z());
  glVertex3f(max.x(),max.y(),min.z());
  glVertex3f(max.x(),min.y(),min.z());
  glVertex3f(max.x(),max.y(),min.z());
  glVertex3f(min.x(),max.y(),min.z());

  glVertex3f(min.x(),min.y(),min.z());
  glVertex3f(min.x(),min.y(),max.z());
  glVertex3f(min.x(),max.y(),min.z());
  glVertex3f(min.x(),max.y(),max.z());
  glVertex3f(max.x(),min.y(),min.z());
  glVertex3f(max.x(),min.y(),max.z());
  glVertex3f(max.x(),max.y(),min.z());
  glVertex3f(max.x(),max.y(),max.z());

  glVertex3f(min.x(),min.y(),max.z());
  glVertex3f(max.x(),min.y(),max.z());
  glVertex3f(min.x(),min.y(),max.z());
  glVertex3f(min.x(),max.y(),max.z());
  glVertex3f(max.x(),max.y(),max.z());
  glVertex3f(max.x(),min.y(),max.z());
  glVertex3f(max.x(),max.y(),max.z());
  glVertex3f(min.x(),max.y(),max.z());

  glEnd();
  glEnable(GL_LIGHTING);	   
}

// ====================================================================
// ====================================================================
