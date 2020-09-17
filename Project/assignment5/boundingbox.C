#include "boundingbox.h"
#include <GL/gl.h>
#include <GL/glu.h>

// ====================================================================
// ====================================================================

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
