// Included files for OpenGL Rendering
#include <GL/gl.h>
#include <GL/glut.h>


#include "system.h"
#include "generator.h"
#include "integrator.h"
#include "forcefield.h"
#include "particleSet.h"
#include "particle.h"

// ====================================================================

System::System(Generator *g, Integrator *i, ForceField *f) {
  assert (g != NULL);
  assert (i != NULL);
  assert (f != NULL);
  generator = g;
  integrator = i;
  forcefield = f;
  particles = NULL;
  Restart();
}

System::~System() {
  delete generator;
  delete integrator;
  delete forcefield;
  delete particles;
}

// ====================================================================

void System::Restart() {
  // delete the current particles
  delete particles;
  particles = new ParticleSet(100);

  // restart the generator (it might have state)
  generator->Restart();

  // reset the main clock
  current_time = 0;
}

void System::Update(float dt) {
  // update each particle
  int num_particles = particles->getNumParticles();
  for (int i = 0; i < num_particles; i++) {
    integrator->Update(particles->Get(i),forcefield,current_time,dt);
  }

  // generate new particles
  int num_new = generator->numNewParticles(current_time,dt);
  for (int i = 0; i < num_new; i++) {
    Particle *p = generator->Generate(current_time,i);
    assert (p != NULL);
    particles->Add(p);
  }

  // cleanup anything that might have died!
  particles->RemoveDead();

  // increment the current time
  current_time += dt;
}


// ====================================================================

void System::PaintGeometry() const {
  // some generators have polygons
  generator->Paint();
}


void System::Paint(float dt, int integrator_color, int draw_vectors, 
		   float acceleration_scale, int motion_blur) const {

  // setup the integrator color (if appropriate)
  if (integrator_color) {
    Vec3f c = integrator->getColor();
    glColor3f(c.r(),c.g(),c.b());
  }

  // draw the particles
  // (and the velocity vectors -- if requested)
  int num_particles = particles->getNumParticles();
  for (int i = 0; i < num_particles; i++) {
    particles->Get(i)->Paint(dt, integrator_color, draw_vectors, motion_blur);
  }

  // draw the acceleration vectors in white -- if requested
  if (draw_vectors) {
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    for (int i = 0; i < num_particles; i++) {
      Particle *p = particles->Get(i);
      Vec3f a = p->getPosition();
      Vec3f b = forcefield->getAcceleration(a,p->getMass(),current_time);
      // scale the acceleration (it's often too large to visualize directly)
      b *= acceleration_scale;
      b += a;
      glVertex3f(a.x(),a.y(),a.z());
      glVertex3f(b.x(),b.y(),b.z());
    }
    glEnd();
  }
}

// ====================================================================
