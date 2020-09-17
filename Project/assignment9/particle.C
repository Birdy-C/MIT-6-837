// Included files for OpenGL Rendering
#include <GL/gl.h>
#include <GL/glut.h>

#include "particle.h"


void Particle::increaseAge(float a) { 
  // interpolate towards dead_color as the clock ticks down  
  float t = 1;
  if (timeToDeath > 0) 
    t = a / timeToDeath;
  if (t > 1) t = 1;
  color += t*(dead_color-color); 

  // adjust the clock
  timeToDeath -= a; 
}


void Particle::Paint(float dt, int integrator_color, int draw_vectors, int motion_blur) const {

  // grab the particle color
  if (integrator_color == 0) {
    glColor3f(color.x(),color.y(),color.z());
  }

  // use slightly larger points if it's not motion blurred
  if (motion_blur == 0) {
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex3f(position.x(),position.y(),position.z());
    glEnd();
  }

  // draw the velocity vectors
  if (draw_vectors == 1) {
    glLineWidth(1);
    glBegin(GL_LINES);
    Vec3f a = position;
    Vec3f b = position + dt*velocity;
    glVertex3f(a.x(),a.y(),a.z());
    glVertex3f(b.x(),b.y(),b.z());
    glEnd();
  }

  // draw motion blur lines
  if (motion_blur == 1) {
    glLineWidth(1);
    glBegin(GL_LINES);
    Vec3f a = position;
    Vec3f b = last_position;
    glVertex3f(a.x(),a.y(),a.z());
    glVertex3f(b.x(),b.y(),b.z());
    glEnd();
  }
}


